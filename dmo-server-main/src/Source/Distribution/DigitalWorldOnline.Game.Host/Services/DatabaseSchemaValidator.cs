using DigitalWorldOnline.Infraestructure;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Serilog;

namespace DigitalWorldOnline.Game.Services
{
    public sealed class DatabaseSchemaValidator
    {
        private static readonly RequiredColumn[] RequiredColumns =
        {
            new("Shared_ItemStorageCharacter", "UpdatedAt"),
            new("Shared_ItemStorageCharacterSlot", "UpdatedAt"),
            new("Shared_ItemStorageAccount", "UpdatedAt"),
            new("Shared_ItemStorageAccountSlot", "UpdatedAt"),
            new("Shared_ItemStorageInstance", "Id")
        };

        private readonly IServiceProvider _serviceProvider;
        private readonly ILogger _logger;

        public DatabaseSchemaValidator(IServiceProvider serviceProvider, ILogger logger)
        {
            _serviceProvider = serviceProvider;
            _logger = logger;
        }

        public async Task ValidateAsync()
        {
            using var scope = _serviceProvider.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<DatabaseContext>();
            var connection = context.Database.GetDbConnection();

            await context.Database.OpenConnectionAsync();
            try
            {
                var databaseName = connection.Database;
                var missing = new List<string>();

                foreach (var required in RequiredColumns)
                {
                    await using var command = connection.CreateCommand();
                    command.CommandText = @"
SELECT COUNT(*)
FROM INFORMATION_SCHEMA.COLUMNS
WHERE TABLE_SCHEMA = DATABASE()
  AND TABLE_NAME = @table
  AND COLUMN_NAME = @column;";

                    var table = command.CreateParameter();
                    table.ParameterName = "@table";
                    table.Value = required.Table;
                    command.Parameters.Add(table);

                    var column = command.CreateParameter();
                    column.ParameterName = "@column";
                    column.Value = required.Column;
                    command.Parameters.Add(column);

                    var count = Convert.ToInt32(await command.ExecuteScalarAsync());
                    if (count == 0)
                        missing.Add($"{required.Table}.{required.Column}");
                }

                if (missing.Count > 0)
                {
                    throw new InvalidOperationException(
                        $"Database '{databaseName}' is missing required schema objects: {string.Join(", ", missing)}. " +
                        "Apply EF migrations before starting the game server.");
                }

                _logger.Information("Database schema validation passed for {Database}.", databaseName);
            }
            finally
            {
                await context.Database.CloseConnectionAsync();
            }
        }

        private sealed record RequiredColumn(string Table, string Column);
    }
}
