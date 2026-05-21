using DigitalWorldOnline.Commons.DTOs.Character;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Builders;

namespace DigitalWorldOnline.Infraestructure.ContextConfiguration.Character
{
    public class CharacterXmlUnionProgressConfiguration : IEntityTypeConfiguration<CharacterXmlUnionProgressDTO>
    {
        public void Configure(EntityTypeBuilder<CharacterXmlUnionProgressDTO> builder)
        {
            builder
                .ToTable("Character_XmlUnionProgress")
                .HasKey(x => x.Id);

            builder
                .Property(x => x.Level)
                .HasColumnType("tinyint unsigned")
                .HasDefaultValue((byte)0)
                .IsRequired();

            builder
                .Property(x => x.CurrentExperience)
                .HasColumnType("int")
                .HasDefaultValue(0)
                .IsRequired();

            builder
                .Property(x => x.RequiredExperience)
                .HasColumnType("int")
                .HasDefaultValue(10000)
                .IsRequired();

            builder
                .Property(x => x.ClaimedRewardMask)
                .HasColumnType("smallint unsigned")
                .HasDefaultValue((ushort)0)
                .IsRequired();

            builder
                .Property(x => x.CreatedAt)
                .HasColumnType("datetime(6)")
                .HasDefaultValueSql("CURRENT_TIMESTAMP(6)")
                .IsRequired();

            builder
                .Property(x => x.UpdatedAt)
                .HasColumnType("datetime(6)")
                .HasDefaultValueSql("CURRENT_TIMESTAMP(6)")
                .IsRequired();

            builder
                .HasIndex(x => x.CharacterId)
                .IsUnique();
        }
    }
}
