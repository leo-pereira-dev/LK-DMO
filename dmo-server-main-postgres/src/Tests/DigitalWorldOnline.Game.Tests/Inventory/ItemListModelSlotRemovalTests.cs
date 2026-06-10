using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Models.Base;
using Xunit;

namespace DigitalWorldOnline.Game.Tests.Inventory;

public sealed class ItemListModelSlotRemovalTests
{
    [Fact]
    public void RemoveOrReduceItem_with_slot_rejects_when_slot_contains_different_item()
    {
        var inventory = new ItemListModel(ItemListEnum.Inventory);
        SetSlot(inventory, slot: 3, itemId: 222, amount: 5);

        var itemToRemove = Item(itemId: 111, amount: 1);

        var removed = inventory.RemoveOrReduceItem(itemToRemove, amount: 1, slot: 3);

        var slot = inventory.FindItemBySlot(3);
        Assert.False(removed);
        Assert.Equal(222, slot.ItemId);
        Assert.Equal(5, slot.Amount);
    }

    [Fact]
    public void RemoveOrReduceItem_with_slot_rejects_when_amount_is_insufficient()
    {
        var inventory = new ItemListModel(ItemListEnum.Inventory);
        SetSlot(inventory, slot: 3, itemId: 111, amount: 1);

        var itemToRemove = Item(itemId: 111, amount: 2);

        var removed = inventory.RemoveOrReduceItem(itemToRemove, amount: 2, slot: 3);

        var slot = inventory.FindItemBySlot(3);
        Assert.False(removed);
        Assert.Equal(111, slot.ItemId);
        Assert.Equal(1, slot.Amount);
    }

    [Fact]
    public void RemoveOrReduceItem_with_slot_reduces_only_the_requested_stack()
    {
        var inventory = new ItemListModel(ItemListEnum.Inventory);
        SetSlot(inventory, slot: 3, itemId: 111, amount: 3);
        SetSlot(inventory, slot: 4, itemId: 111, amount: 10);

        var itemToRemove = Item(itemId: 111, amount: 2);

        var removed = inventory.RemoveOrReduceItem(itemToRemove, amount: 2, slot: 3);

        Assert.True(removed);
        Assert.Equal(111, inventory.FindItemBySlot(3).ItemId);
        Assert.Equal(1, inventory.FindItemBySlot(3).Amount);
        Assert.Equal(111, inventory.FindItemBySlot(4).ItemId);
        Assert.Equal(10, inventory.FindItemBySlot(4).Amount);
    }

    [Fact]
    public void RemoveOrReduceItemsByItemIds_consumes_across_allowed_item_ids_only()
    {
        var inventory = new ItemListModel(ItemListEnum.Inventory);
        SetSlot(inventory, slot: 3, itemId: 9400, amount: 4);
        SetSlot(inventory, slot: 4, itemId: 41002, amount: 6);
        SetSlot(inventory, slot: 5, itemId: 41000, amount: 20);

        var removed = inventory.RemoveOrReduceItemsByItemIds(new[] { 9400, 41002 }, totalAmount: 7);

        Assert.True(removed);
        Assert.Equal(0, inventory.FindItemBySlot(3).ItemId);
        Assert.Equal(0, inventory.FindItemBySlot(3).Amount);
        Assert.Equal(41002, inventory.FindItemBySlot(4).ItemId);
        Assert.Equal(3, inventory.FindItemBySlot(4).Amount);
        Assert.Equal(41000, inventory.FindItemBySlot(5).ItemId);
        Assert.Equal(20, inventory.FindItemBySlot(5).Amount);
    }

    [Fact]
    public void RemoveOrReduceItemsByItemIds_reverts_when_allowed_items_are_insufficient()
    {
        var inventory = new ItemListModel(ItemListEnum.Inventory);
        SetSlot(inventory, slot: 3, itemId: 9400, amount: 2);
        SetSlot(inventory, slot: 4, itemId: 41002, amount: 3);
        SetSlot(inventory, slot: 5, itemId: 41000, amount: 20);

        var removed = inventory.RemoveOrReduceItemsByItemIds(new[] { 9400, 41002 }, totalAmount: 6);

        Assert.False(removed);
        Assert.Equal(9400, inventory.FindItemBySlot(3).ItemId);
        Assert.Equal(2, inventory.FindItemBySlot(3).Amount);
        Assert.Equal(41002, inventory.FindItemBySlot(4).ItemId);
        Assert.Equal(3, inventory.FindItemBySlot(4).Amount);
        Assert.Equal(41000, inventory.FindItemBySlot(5).ItemId);
        Assert.Equal(20, inventory.FindItemBySlot(5).Amount);
    }

    private static void SetSlot(ItemListModel inventory, int slot, int itemId, int amount)
    {
        var item = inventory.FindItemBySlot(slot);
        item.SetItemId(itemId);
        item.SetAmount(amount);
    }

    private static ItemModel Item(int itemId, int amount)
    {
        var item = new ItemModel();
        item.SetItemId(itemId);
        item.SetAmount(amount);
        return item;
    }
}
