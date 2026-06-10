using DigitalWorldOnline.Commons.Enums;

namespace DigitalWorldOnline.Commons.Models.Base
{
    public partial class ItemAccessoryStatusModel
    {
        public int EffectiveValue => Value > 0 ? Value : 0;

        public ushort WireValue => Value > 0 ? (ushort)Value : (ushort)0;

        public bool HasInvalidNegativeValue => Value < 0;

        /// <summary>
        /// Sets the status type enumeration.
        /// </summary>
        /// <param name="type"></param>
        public void SetType(AccessoryStatusTypeEnum type) => Type = type;

        /// <summary>
        /// Sets the status value.
        /// </summary>
        /// <param name="value"></param>
        public void SetValue(short value) => Value = value < 0 ? (short)0 : value;

        public void SetValue(int value) => Value = (short)Math.Clamp(value, 0, short.MaxValue);
    }
}
