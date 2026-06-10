using DigitalWorldOnline.Commons.Models.Base;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Update
{
    public class UpdateItemsCommand : IRequest
    {
        public List<ItemModel> Items { get; }
        public bool PreserveUnreferencedInstances { get; }

        public UpdateItemsCommand(List<ItemModel> items, bool preserveUnreferencedInstances = false)
        {
            Items = items;
            PreserveUnreferencedInstances = preserveUnreferencedInstances;
        }

        public UpdateItemsCommand(ItemListModel itemList, bool preserveUnreferencedInstances = false)
        {
            Items = itemList.Items;
            PreserveUnreferencedInstances = preserveUnreferencedInstances;
        }
    }
}
