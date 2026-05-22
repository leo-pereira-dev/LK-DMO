using AutoMapper;
using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Account;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Diagnostics;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;
using static Microsoft.EntityFrameworkCore.DbLoggerCategory.Database;
using System.IO;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class InitialInformationPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.InitialInformation;

        private readonly PartyManager _partyManager;
        private readonly StatusManager _statusManager;
        private readonly MapServer _mapServer;
        private readonly PvpServer _pvpServer;
        private readonly DungeonsServer _dungeonsServer;

        private readonly AssetsLoader _assets;
        private readonly DigimonEvoBinLoader _digimonEvo;
        private readonly DUnitCollectionService _dUnitCollections;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly IMapper _mapper;

        public InitialInformationPacketProcessor(
            PartyManager partyManager,
            StatusManager statusManager,
            MapServer mapServer,
            PvpServer pvpServer,
            DungeonsServer dungeonsServer,
            AssetsLoader assets,
            DigimonEvoBinLoader digimonEvo,
            DUnitCollectionService dUnitCollections,
            ILogger logger,
            ISender sender,
            IMapper mapper)
        {
            _partyManager = partyManager;
            _statusManager = statusManager;
            _mapServer = mapServer;
            _pvpServer = pvpServer;
            _dungeonsServer = dungeonsServer;
            _assets = assets;
            _digimonEvo = digimonEvo;
            _dUnitCollections = dUnitCollections;
            _logger = logger;
            _sender = sender;
            _mapper = mapper;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            _logger.Debug("Getting packet parameters...");
            packet.Skip(4);
            var accountId = packet.ReadUInt();
            PortalTrace.Write($"InitialInformation request account={accountId}");

            _logger.Debug($"{accountId}");

            _logger.Debug($"Getting account information...");
            _logger.Information($"Searching account with id {accountId}...");
            var account = _mapper.Map<AccountModel>(await _sender.Send(new AccountByIdQuery(accountId)));
            client.SetAccountInfo(account);

            _logger.Debug($"Getting character base information...");
            _logger.Information($"Searching character with id {account.LastPlayedCharacter} for account {account.Id}...");
            var character = _mapper.Map<CharacterModel>(await _sender.Send(new CharacterByIdQuery(account.LastPlayedCharacter)));
            if (character.Partner == null)
            {
                _logger.Error($"Invalid character information for tamer id {account.LastPlayedCharacter}.");
                return;
            }
            PortalTrace.Write($"InitialInformation loaded account={accountId} tamer={character.Id} map={character.Location.MapId} x={character.Location.X} y={character.Location.Y} channel={character.Channel}");

            account.ItemList.ForEach(character.AddItemList);

            // Backfill the four standard account-level ItemLists for legacy accounts
            // (created before AccountModel.Create wired them up). For each missing type:
            // 1. Persist a Shared_ItemList row (idempotent via EnsureAccountItemListAsync —
            //    no-op if the row already exists from a prior session).
            // 2. Add the in-memory model so the current ComplementarInformation handler
            //    can use it without crashing on null.
            //
            // Without this, ComplementarInformationPacketProcessor's LoadInventoryPacket
            // call hits a null AccountWarehouse, throws NRE inside ProcessPacketAsync
            // (which OnDataReceivedEvent fires-and-forgets, swallowing the exception),
            // the handler aborts mid-flight and the character never transitions to
            // CharacterStateEnum.Ready — leaving them invisible to MonsterOperation, so
            // no mobs ever spawn for the player.
            async Task EnsureListAsync(ItemListEnum t)
            {
                if (account.ItemList.Any(x => x.Type == t)) return;
                await _sender.Send(new CreateAccountItemListCommand(account.Id, t));

                var refreshedAccount = _mapper.Map<AccountModel>(await _sender.Send(new AccountByIdQuery(account.Id)));
                var persistedList = refreshedAccount.ItemList.FirstOrDefault(x => x.Type == t);

                if (persistedList != null)
                {
                    character.AddItemList(persistedList);
                    return;
                }
                
                _logger.Error("Account list creation did not materialize persisted list. accountId={AccountId} type={Type}", account.Id, t);
                throw new InvalidOperationException($"Missing persisted account item list {t} for account {account.Id}.");
            }
            await EnsureListAsync(ItemListEnum.AccountWarehouse);
            await EnsureListAsync(ItemListEnum.CashWarehouse);
            await EnsureListAsync(ItemListEnum.ShopWarehouse);
            await EnsureListAsync(ItemListEnum.BuyHistory);

            // Character-owned required lists (legacy/migrated tamers can be missing
            // rows after item-system transition). Keep the runtime model complete so
            // stats/packets that require Equipment/Inventory etc. never explode.
            async Task EnsureCharacterListAsync(ItemListEnum type)
            {
                if (character.ItemList.Any(x => x.Type == type))
                    return;

                await _sender.Send(new CreateCharacterItemListCommand(character.Id, type));

                var refreshedCharacter = _mapper.Map<CharacterModel>(await _sender.Send(new CharacterByIdQuery(character.Id)));
                var persistedList = refreshedCharacter.ItemList.FirstOrDefault(x => x.Type == type);
                if (persistedList != null)
                {
                    character.AddItemList(persistedList);
                    return;
                }

                _logger.Error("Character list creation did not materialize persisted list. characterId={CharacterId} type={Type}", character.Id, type);
                throw new InvalidOperationException($"Missing persisted character item list {type} for character {character.Id}.");
            }

            await EnsureCharacterListAsync(ItemListEnum.Equipment);
            await EnsureCharacterListAsync(ItemListEnum.Inventory);
            await EnsureCharacterListAsync(ItemListEnum.Warehouse);
            await EnsureCharacterListAsync(ItemListEnum.Chipsets);
            await EnsureCharacterListAsync(ItemListEnum.JogressChipset);
            await EnsureCharacterListAsync(ItemListEnum.Digivice);
            await EnsureCharacterListAsync(ItemListEnum.TamerSkill);
            await EnsureCharacterListAsync(ItemListEnum.RewardWarehouse);
            await EnsureCharacterListAsync(ItemListEnum.GiftWarehouse);
            await EnsureCharacterListAsync(ItemListEnum.ConsignedWarehouse);
            await EnsureCharacterListAsync(ItemListEnum.TamerShop);
            await EnsureCharacterListAsync(ItemListEnum.ConsignedShop);

            var duplicateItemListTypes = character.ItemList
                .GroupBy(x => x.Type)
                .Where(x => x.Count() > 1)
                .Select(x => $"{x.Key}:{x.Count()}")
                .ToList();

            if (duplicateItemListTypes.Any())
            {
                _logger.Warning(
                    "Character {CharacterId} loaded duplicate item list types after hydration: {DuplicateTypes}",
                    character.Id,
                    string.Join(",", duplicateItemListTypes));
            }

            _logger.Information(
                "Character {CharacterId} inventory hydrated with size {InventorySize}, item rows {InventoryRows}, occupied {InventoryOccupied}.",
                character.Id,
                character.Inventory.Size,
                character.Inventory.Items.Count,
                character.Inventory.Count);

            await RecoverInvalidEvolutionState(character);
            await RecoverAutoUnlockedEvolutions(character);

            foreach (var digimon in character.Digimons)
            {
                digimon.SetTamer(character);

                digimon.SetBaseInfo(
                    _statusManager.GetDigimonBaseInfo(
                        digimon.CurrentType
                    )
                );

                digimon.SetBaseStatus(
                    _statusManager.GetDigimonBaseStatus(
                        digimon.CurrentType,
                        digimon.Level,
                        digimon.Size
                    )
                );

                digimon.SetTitleStatus(
                    _statusManager.GetTitleStatus(
                        character.CurrentTitle
                    )
                );

                digimon.SetSealStatus(_assets.SealInfo);
            }

            await HydrateArchiveDigimonsForDUnit(character);

            _logger.Debug($"Getting character status information...");
            // Per-model tamer "base status" retired — DMBase.bin §1 carries the full per-level
            // stat block, equipment/socket/buff add on top in CharacterModelBehavior. v487 client
            // never had a separate per-model baseline. See LevelingStatus path below.
            //character.SetBaseStatus(
            //    _statusManager.GetTamerBaseStatus(
            //        character.Model
            //    )
            //);

            character.SetLevelStatus(
                _statusManager.GetTamerLevelStatus(
                    character.Model,
                    character.Level
                )
            );

            _dUnitCollections.ApplyBonuses(character);

            character.NewViewLocation(character.Location.X, character.Location.Y);
            character.RemovePartnerPassiveBuff();
            character.SetPartnerPassiveBuff();

            await _sender.Send(new UpdateDigimonBuffListCommand(character.Partner.BuffList));
            
            _logger.Information($"Updating itens and buff status for character {character.Id}...");
            _logger.Debug($"Concatting character items information...");
            foreach (var item in character.ItemList.SelectMany(x => x.Items).Where(x => x.ItemId > 0))
                item.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == item?.ItemId));

            _logger.Debug($"Concatting buffs information...");
            foreach (var buff in character.BuffList.ActiveBuffs)
                buff.SetBuffInfo(_assets.BuffInfo.FirstOrDefault(x => x.SkillCode == buff.SkillId || x.DigimonSkillCode == buff.SkillId));

            foreach (var buff in character.Partner.BuffList.ActiveBuffs)
                buff.SetBuffInfo(_assets.BuffInfo.FirstOrDefault(x => x.SkillCode == buff.SkillId || x.DigimonSkillCode == buff.SkillId));

            _logger.Information($"Waiting an available channel for character {character.Id}...");
            _logger.Debug($"Getting available channels...");

            //var channels = await _sender.Send(new ChannelsByMapIdQuery(character.Location.MapId));
            //
            //byte? channel = character.Channel == byte.MaxValue ?
            //    channels?
            //    .OrderByDescending(x => x.Value)
            //    .FirstOrDefault(x => x.Value < byte.MaxValue)
            //    .Key : character.Channel;
            //
            //while (channel == null)
            //{
            //    if (channels?.Count > 15)
            //        break;
            //
            //    _logger.Debug($"Creating new channel for map {character.Location.MapId}...");
            //    channels?.Add(channels.Keys.GetNewChannel(), 0);
            //
            //    channel = character.Channel == byte.MaxValue ?
            //    channels?
            //    .OrderByDescending(x => x.Value)
            //    .FirstOrDefault(x => x.Value < byte.MaxValue)
            //    .Key : character.Channel;
            //}

            // Phase E: do NOT hard-reset Channel to 0 — that overrides a switch
            // request mid-flight.  MapServer.AddClient -> PickChannelFor honours
            // the persisted Channel when it points at a live initialized
            // channel of the target map, and falls back to lowest-populated
            // otherwise.

            character.UpdateState(CharacterStateEnum.Loading);
            character.EnsureXmlUnionProgress();
            character.XmlUnionProgress.SetProgress(
                character.XmlUnionProgress.Level,
                character.XmlUnionProgress.CurrentExperience,
                _assets.XmlUnion.GetRequiredExperience(character.XmlUnionProgress.Level));

            client.SetCharacter(character);

            _logger.Debug($"Updating character state...");
            await _sender.Send(new UpdateCharacterStateCommand(character.Id, CharacterStateEnum.Loading));
            await _sender.Send(new UpdateCharacterXmlUnionProgressCommand(character.XmlUnionProgress));

            if (character.Location.MapId == 9101)
            {
                _logger.Information($"Adding character {character.Id} to PvP map {character.Location.MapId}...");
                _pvpServer.AddClient(client);
            }
            else if (client.DungeonMap)
            {
                _logger.Information($"Adding character {character.Id} to map {character.Location.MapId}...");
                _dungeonsServer.AddClient(client);
            }
            else
            {
                _logger.Information($"Adding character {character.Id} to map {character.Location.MapId}...");
                _mapServer.AddClient(client);
            }
            PortalTrace.Write($"InitialInformation AddClient tamer={character.Id} map={character.Location.MapId} channel={character.Channel} loading={client.Loading}");

            while (client.Loading) await Task.Delay(1000);
            PortalTrace.Write($"InitialInformation loading complete tamer={character.Id} map={character.Location.MapId}");

            character.SetGenericHandler(character.Partner.GeneralHandler);

            var party = _partyManager.FindParty(client.TamerId);
            if (party != null)
            {
                party.UpdateMember(party[client.TamerId]);
            }

            if (!client.DungeonMap)
            {
                var region = _assets.Maps.FirstOrDefault(x => x.MapId == character.Location.MapId);

                if (region != null)
                {
                    if (character.MapRegions[region.RegionIndex].Unlocked != 0x80)
                    {
                        var characterRegion = character.MapRegions[region.RegionIndex];
                        characterRegion.Unlock();

                        await _sender.Send(new UpdateCharacterMapRegionCommand(characterRegion));
                    }
                }
            }

            await ReceiveArenaPoints(client);

            // EvoSlot lookup: client's CDigimonEvolveObj::m_nEvoSlot comes from
            // DigimonEvo.bin's "Ev0_num" column — a 1-based per-tree slot index,
            // NOT the global nEvo:: enum value from Digimon_List.bin's s_eEvolutionType.
            // (CDigimonEvolution.cpp:170 — pEvolveObj->m_nEvoSlot = atoi(... "Ev0_num"))
            // We scan every tree's DigimonEvoLine list to find the form's EvoSlot;
            // returns 0 when unmapped so the packet writer falls back to (i+1).
            byte EvoSlotFor(int formType)
            {
                foreach (var tree in _digimonEvo.Data.ByType.Values)
                {
                    foreach (var line in tree.Lines)
                    {
                        if (line.Type == formType)
                            return (byte)line.EvoSlot;
                    }
                }
                return 0;
            }

            LogInitialInfoDiagnostics(character, EvoSlotFor);

            var __pkt = new InitialInfoPacket(character, party, EvoSlotFor, account.AccessLevel);
            var __bytes = __pkt.Serialize();

            client.Send(__bytes);
            PortalTrace.Write($"InitialInfoPacket sent tamer={character.Id} bytes={__bytes.Length} map={character.Location.MapId}");
            _logger.Information(
                "[INIT-TRACE] sent tamer={TamerId} bytes={Bytes} map={MapId} channel={Channel} tamerHandler={TamerHandler} partnerHandler={PartnerHandler}",
                character.Id,
                __bytes.Length,
                character.Location.MapId,
                character.Channel,
                character.GeneralHandler,
                character.Partner.GeneralHandler);

            // Diagnostic dump — failure must never block gameplay
            try
            {
                var dumpDir = "PacketDumps";
                Directory.CreateDirectory(dumpDir);
                var dumpPath = Path.Combine(dumpDir, $"initgamedata_tamer{character.Id}_{DateTime.UtcNow:yyyyMMddHHmmssfff}.bin");
                File.WriteAllBytes(dumpPath, __bytes);
                _logger.Information($"Dumped InitialInfoPacket: {__bytes.Length} bytes -> {dumpPath}");
            }
            catch (Exception ex)
            {
                _logger.Warning($"Could not dump InitialInfoPacket: {ex.Message}");
            }



            _logger.Debug($"Updating character channel...");
            await _sender.Send(new UpdateCharacterChannelCommand(character.Id, character.Channel));
        }

        private void LogInitialInfoDiagnostics(CharacterModel character, Func<int, byte> evoSlotFor)
        {
            _logger.Information(
                "[INIT-TRACE] build tamer={TamerId} name={Name} map={MapId} pos={X},{Y} state={State} level={Level} hp={Hp}/{MaxHp} ds={Ds}/{MaxDs} partner={PartnerId}:{PartnerName} base={BaseType} current={CurrentType} level={PartnerLevel} slots={DigimonSlots} activeEvoDs={ActiveEvoDs} activeEvoXg={ActiveEvoXg}",
                character.Id,
                character.Name,
                character.Location.MapId,
                character.Location.X,
                character.Location.Y,
                character.State,
                character.Level,
                character.CurrentHp,
                character.HP,
                character.CurrentDs,
                character.DS,
                character.Partner.Id,
                character.Partner.Name,
                character.Partner.BaseType,
                character.Partner.CurrentType,
                character.Partner.Level,
                character.DigimonSlots,
                character.ActiveEvolution.DsPerSecond,
                character.ActiveEvolution.XgPerSecond);

            _logger.Information(
                "[INIT-TRACE] tamerBuffs count={Count} data={Buffs}",
                character.BuffList.ActiveBuffs.Count,
                BuildBuffSummary(character.BuffList.ActiveBuffs));

            _logger.Information(
                "[INIT-TRACE] partnerBuffs count={Count} data={Buffs}",
                character.Partner.BuffList.ActiveBuffs.Count,
                BuildBuffSummary(character.Partner.BuffList.ActiveBuffs));

            _logger.Information(
                "[INIT-TRACE] partnerEvos count={Count} data={Evos}",
                character.Partner.Evolutions.Count,
                string.Join("|", character.Partner.Evolutions.Select(x =>
                    $"{x.Type}:unlocked={x.Unlocked}:slot={evoSlotFor(x.Type)}:skills={x.Skills.Count}:mem={x.MemorySkills.Count}")));

            _logger.Information(
                "[INIT-TRACE] activeSkills count={Count} data={Skills}",
                character.ActiveSkill.Count,
                string.Join("|", character.ActiveSkill.Select(x =>
                    $"{x.Type}:{x.SkillId}:cd={x.RemainingCooldownSeconds}:min={x.RemainingMinutes}:dur={x.Duration}:cool={x.Cooldown}")));
        }

        private static string BuildBuffSummary(IEnumerable<DigitalWorldOnline.Commons.Models.Buff> buffs)
        {
            return string.Join("|", buffs.Select(x =>
                $"{x.BuffId}:{x.SkillId}:type={x.TypeN}:dur={x.Duration}:remain={x.RemainingSeconds}:buffInfo={(x.BuffInfo == null ? 0 : 1)}"));
        }

        private async Task HydrateArchiveDigimonsForDUnit(CharacterModel character)
        {
            foreach (var digimonArchive in character.DigimonArchive.DigimonArchives.Where(x => x.DigimonId > 0))
            {
                if (digimonArchive.Digimon != null)
                    continue;

                digimonArchive.SetDigimonInfo(_mapper.Map<DigitalWorldOnline.Commons.Models.Digimon.DigimonModel>(
                    await _sender.Send(new GetDigimonByIdQuery(digimonArchive.DigimonId))));

                if (digimonArchive.Digimon == null)
                    continue;

                digimonArchive.Digimon.SetTamer(character);
                digimonArchive.Digimon.SetBaseInfo(
                    _statusManager.GetDigimonBaseInfo(digimonArchive.Digimon.BaseType));
                digimonArchive.Digimon.SetBaseStatus(
                    _statusManager.GetDigimonBaseStatus(
                        digimonArchive.Digimon.BaseType,
                        digimonArchive.Digimon.Level,
                        digimonArchive.Digimon.Size));
            }

            _logger.Information(
                "[DUnit] Character {CharacterId} archive digimons available for collection calculation: {Count}",
                character.Id,
                character.DigimonArchive.DigimonArchives.Count(x => x.Digimon != null));
        }

        private async Task ReceiveArenaPoints(GameClient client)
        {
            if (client.Tamer.Points.Amount > 0)
            {
                var newItem = new ItemModel();
                newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == client.Tamer.Points.ItemId));



                newItem.ItemId = client.Tamer.Points.ItemId;
                newItem.Amount = client.Tamer.Points.Amount;

                if (newItem.IsTemporary)
                    newItem.SetRemainingTime((uint)newItem.ItemInfo.UsageTimeMinutes);

                var itemClone = (ItemModel)newItem.Clone();

                if (client.Tamer.Inventory.AddItem(newItem))
                {
                    await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                }
                else
                {
                    client.Tamer.GiftWarehouse.AddItem(newItem);
                    await _sender.Send(new UpdateItemsCommand(client.Tamer.GiftWarehouse));
                }

                client.Tamer.Points.SetAmount(0);
                client.Tamer.Points.SetCurrentStage(0);

                await _sender.Send(new UpdateCharacterArenaPointsCommand(client.Tamer.Points));
            }
            else if (client.Tamer.Points.CurrentStage > 0)
            {
                client.Tamer.Points.SetCurrentStage(0);
                await _sender.Send(new UpdateCharacterArenaPointsCommand(client.Tamer.Points));
            }
        }

        private async Task RecoverInvalidEvolutionState(CharacterModel character)
        {
            var recovered = false;

            foreach (var digimon in character.Digimons)
            {
                if (digimon.CurrentType == digimon.BaseType)
                    continue;

                var currentEvolution = digimon.Evolutions.FirstOrDefault(x => x.Type == digimon.CurrentType);
                if (currentEvolution != null && currentEvolution.Unlocked > 0)
                    continue;

                _logger.Warning(
                    "Character {CharacterId} digimon {DigimonId} had invalid current evolution {CurrentType} (base {BaseType}, unlocked={Unlocked}); reverting to base type.",
                    character.Id,
                    digimon.Id,
                    digimon.CurrentType,
                    digimon.BaseType,
                    currentEvolution?.Unlocked ?? 0);

                digimon.UpdateCurrentType(digimon.BaseType);
                await _sender.Send(new UpdatePartnerCurrentTypeCommand(digimon));
                recovered = true;
            }

            if (!recovered)
                return;

            character.ActiveEvolution.SetDs(0);
            character.ActiveEvolution.SetXg(0);
            await _sender.Send(new UpdateCharacterActiveEvolutionCommand(character.ActiveEvolution));
        }

        private async Task RecoverAutoUnlockedEvolutions(CharacterModel character)
        {
            foreach (var digimon in character.Digimons)
            {
                foreach (var evolution in digimon.Evolutions.Where(x => x.Unlocked == 0))
                {
                    var line = _digimonEvo.Data.FindByType(digimon.BaseType)?.Lines.FirstOrDefault(x => x.Type == evolution.Type);
                    if (line == null || !ShouldAutoUnlockEvolution(line))
                        continue;

                    evolution.Unlock();
                    await _sender.Send(new UpdateEvolutionCommand(evolution));

                    _logger.Information(
                        "[EVO-REPAIR] character={CharacterId} digimon={DigimonId} base={BaseType} unlocked={EvolutionType} slot={SlotLevel}",
                        character.Id,
                        digimon.Id,
                        digimon.BaseType,
                        evolution.Type,
                        line.EvoSlot);
                }
            }
        }

        private static bool ShouldAutoUnlockEvolution(DigimonEvoLine line)
        {
            if (line.EvoSlot <= 2)
                return true;

            return line.EnableSlot > 0 &&
                   line.OpenQualification == 0 &&
                   line.OpenQuest <= 0 &&
                   line.UseItem <= 0 &&
                   line.UseItemNum <= 0;
        }
    }
}
