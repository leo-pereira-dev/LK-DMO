namespace ItemEditor.Models;

public sealed record ItemKindOption(
    string Key,
    string Name,
    string Detail,
    ushort DefaultTypeL,
    ushort DefaultTypeS = 0,
    bool ClearsSkill = false)
{
    public override string ToString() => Name;
}

public static class ItemKindCatalog
{
    public const string Box = "box";
    public const string UsableBuff = "usable_buff";
    public const string Recovery = "recovery";
    public const string Equipment = "equipment";
    public const string Accessory = "accessory";
    public const string AttributeSocket = "attribute_socket";
    public const string MemorySkill = "memory_skill";
    public const string Megaphone = "megaphone";
    public const string Generic = "generic";

    public static IReadOnlyList<ItemKindOption> Options { get; } =
    [
        new(Box, "Box / ScanUse item", "TypeL 170, rewards from Container.bin or dso.sql backup", 170, ClearsSkill: true),
        new(UsableBuff, "Usable buff / status item", "TypeL 63, effect from Skill.bin/Buff.bin", 63),
        new(Recovery, "Recovery consumable", "TypeL 61, usable recovery item", 61),
        new(Equipment, "Equipment / wearable", "TypeL 21-27/31-36/53, can carry stat skill", 26),
        new(Accessory, "Accessory equipment", "TypeL 28-30/33, accessory options/stats", 29),
        new(AttributeSocket, "Socket attribute item", "TypeL 120/122-127, equipment socket attribute", 120),
        new(MemorySkill, "Memory skill chip", "TypeL 67-69, Digimon memory skill chip", 67),
        new(Megaphone, "Megaphone / Shouter", "TypeL 165-169, opens shouter UI and sends global megaphone chat", 165, ClearsSkill: true),
        new(Generic, "Generic item / material", "TypeL 80-86 style non-effect item", 86, ClearsSkill: true)
    ];

    public static ItemKindOption Infer(ItemEditorRow item, bool hasContainer)
    {
        if (hasContainer || item.TypeL == 170)
            return Find(Box);

        if (IsAccessoryType(item.TypeL))
            return Find(Accessory);

        if (IsEquipmentType(item.TypeL))
            return Find(Equipment);

        if (IsAttributeSocketType(item.TypeL))
            return Find(AttributeSocket);

        if (item.TypeL is 67 or 68 or 69)
            return Find(MemorySkill);

        if (IsMegaphoneType(item.TypeL))
            return Find(Megaphone);

        if (item.TypeL is 61 or 71 or 201)
            return Find(Recovery);

        if (item.TypeL is 63 or 64 or 186 or 187 || item.SkillCode > 0)
            return Find(UsableBuff);

        return Find(Generic);
    }

    public static ItemKindOption Find(string key)
        => Options.First(x => x.Key == key);

    public static bool IsEquipmentType(int typeL)
        => typeL is 21 or 22 or 23 or 24 or 25 or 26 or 27 or 31 or 32 or 34 or 36 or 53;

    public static bool IsAccessoryType(int typeL)
        => typeL is 28 or 29 or 30 or 33;

    public static bool IsAttributeSocketType(int typeL)
        => typeL is 120 or 122 or 123 or 124 or 125 or 127;

    public static bool IsMegaphoneType(int typeL)
        => typeL is 165 or 166 or 167 or 168 or 169;

    public static string MegaphoneVariant(int typeL)
        => typeL switch
        {
            165 => "Normal cash shouter",
            166 => "Special cash shouter",
            167 => "Normal timed cash shouter",
            168 => "Special timed cash shouter",
            169 => "GM timed shouter",
            _ => "Megaphone / shouter"
        };
}
