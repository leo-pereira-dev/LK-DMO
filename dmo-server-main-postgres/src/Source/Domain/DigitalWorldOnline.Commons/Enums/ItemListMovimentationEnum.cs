namespace DigitalWorldOnline.Commons.Enums
{
    public enum ItemListMovimentationEnum
    {
        InvalidMovimentation = -1,

        InventoryToInventory = 0,
        InventoryToEquipment = 1,
        InventoryToWarehouse = 2,
        InventoryToAccountWarehouse = 3,

        EquipmentToInventory = 10,
        EquipmentToWarehouse = 11,
        EquipmentToAccountWarehouse = 12,

        WarehouseToWarehouse = 20,
        WarehouseToInventory = 21,
        WarehouseToAccountWarehouse = 22,
        WarehouseToEquipment = 23,

        AccountWarehouseToAccountWarehouse = 30,
        AccountWarehouseToInventory = 31,
        AccountWarehouseToWarehouse = 32,
        AccountWarehouseToEquipment = 33,

        DigiviceToInventory = 40,
        InventoryToDigivice = 41,
        
        InventoryToChipset = 50,
        ChipsetToInventory = 51,

        InventoryToJogressChipset = 52,
        JogressChipsetToInventory = 53,

        InventoryToExtraSeal = 60,
        ExtraSealToInventory = 61,
        ExtraSealToExtraSeal = 62,

        InventoryToExtraTicket = 63,
        ExtraTicketToInventory = 64,
        ExtraTicketToExtraTicket = 65,

        InventoryToExtraEvolution = 66,
        ExtraEvolutionToInventory = 67,
        ExtraEvolutionToExtraEvolution = 68,

        InventoryToExtraDigitama = 69,
        ExtraDigitamaToInventory = 70,
        ExtraDigitamaToExtraDigitama = 71,

        InventoryToExtraMaterial = 72,
        ExtraMaterialToInventory = 73,
        ExtraMaterialToExtraMaterial = 74
    }
}
