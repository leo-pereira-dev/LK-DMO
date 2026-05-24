using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Models.Digimon;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    internal static class ClientDigimonSlotResolver
    {
        public static DigimonModel? Resolve(GameClient client, int clientSlot, out int serverSlot)
        {
            serverSlot = 0;

            if (clientSlot < 0 || clientSlot > byte.MaxValue)
                return null;

            // The client sends 0 for the active partner and 1..N for mercenary slots.
            if (clientSlot == 0 && client.Partner != null)
            {
                serverSlot = client.Partner.Slot;
                return client.Partner;
            }

            var directSlot = (byte)clientSlot;
            var directPartner = client.Tamer.Digimons.FirstOrDefault(x =>
                x.Slot != byte.MaxValue &&
                x.Slot == directSlot);

            if (directPartner != null)
            {
                serverSlot = directPartner.Slot;
                return directPartner;
            }

            var visualPartner = client.Tamer.Digimons
                .Where(x => x.Slot != byte.MaxValue && x.Slot < client.Tamer.DigimonSlots)
                .OrderBy(x => x.Slot)
                .ElementAtOrDefault(clientSlot);

            if (visualPartner != null)
                serverSlot = visualPartner.Slot;

            return visualPartner;
        }
    }
}
