using DigitalWorldOnline.Infraestructure.Extensions;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;

namespace DigitalWorldOnline.Infraestructure
{
    public partial class DatabaseContext : DbContext
    {
        private const string DesignTimeEnvVar = "DSO_DB_CONNECTION";

        static DatabaseContext()
        {
            AppContext.SetSwitch("Npgsql.EnableLegacyTimestampBehavior", true);
        }

        public DatabaseContext()
        {
        }

        public DatabaseContext(DbContextOptions<DatabaseContext> options)
            : base(options)
        {
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            if (optionsBuilder.IsConfigured) return;

            var cs = Environment.GetEnvironmentVariable(DesignTimeEnvVar)
                ?? throw new InvalidOperationException(
                    $"No DB connection string available at design time. Set environment variable '{DesignTimeEnvVar}'.");

            optionsBuilder.UseDsoPostgres(cs);
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            SharedEntityConfiguration(modelBuilder);
            AccountEntityConfiguration(modelBuilder);
            AssetsEntityConfiguration(modelBuilder);
            CharacterEntityConfiguration(modelBuilder);
            ConfigEntityConfiguration(modelBuilder);
            DigimonEntityConfiguration(modelBuilder);
            EventEntityConfiguration(modelBuilder);
            SecurityEntityConfiguration(modelBuilder);
            ShopEntityConfiguration(modelBuilder);
            MechanicsEntityConfiguration(modelBuilder);
            RoutineEntityConfiguration(modelBuilder);
            ArenaEntityConfiguration(modelBuilder);

            ApplyInitialTemporalDefaults(modelBuilder);
        }

        private static void ApplyInitialTemporalDefaults(ModelBuilder modelBuilder)
        {
            foreach (var property in modelBuilder.Model.GetEntityTypes().SelectMany(x => x.GetProperties()))
            {
                if (property.ClrType != typeof(DateTime))
                    continue;

                if (!IsInitialTemporalProperty(property.Name))
                    continue;

                property.IsNullable = false;
                property.SetDefaultValueSql("CURRENT_TIMESTAMP");
            }
        }

        private static bool IsInitialTemporalProperty(string propertyName)
        {
            if (propertyName.Contains("Update", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("End", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Expire", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Expiration", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Cooldown", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Last", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Next", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Delete", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Deleted", StringComparison.OrdinalIgnoreCase) ||
                propertyName.Contains("Exclude", StringComparison.OrdinalIgnoreCase))
                return false;

            return propertyName.Equals("CreateDate", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("CreatedAt", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("CreationDate", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("InsertDate", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("AcquiredAt", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("StartDate", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("StartTime", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("Date", StringComparison.OrdinalIgnoreCase) ||
                   propertyName.Equals("Time", StringComparison.OrdinalIgnoreCase);
        }
    }
}
