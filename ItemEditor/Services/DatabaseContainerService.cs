using System.IO;
using System.Text.Json;
using ItemEditor.Models;
using Npgsql;

namespace ItemEditor.Services;

public sealed class DatabaseContainerService
{
    private static readonly string SettingsDirectory = Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
        "LKDMO.ItemEditor");

    private static readonly string SettingsPath = Path.Combine(SettingsDirectory, "settings.json");

    public string LoadConnectionString()
    {
        try
        {
            if (File.Exists(SettingsPath))
            {
                var settings = JsonSerializer.Deserialize<EditorSettings>(File.ReadAllText(SettingsPath));
                if (!string.IsNullOrWhiteSpace(settings?.DatabaseConnection))
                    return settings.DatabaseConnection;
            }
        }
        catch
        {
            // Ignore invalid local settings and fall back to the workspace .env.
        }

        return TryBuildProductionConnectionString() ?? string.Empty;
    }

    public void SaveConnectionString(string connectionString)
    {
        Directory.CreateDirectory(SettingsDirectory);
        var settings = new EditorSettings { DatabaseConnection = connectionString.Trim() };
        File.WriteAllText(SettingsPath, JsonSerializer.Serialize(settings, new JsonSerializerOptions { WriteIndented = true }));
    }

    public async Task<string> TestConnectionAsync(string connectionString)
    {
        await using var connection = new NpgsqlConnection(connectionString);
        await connection.OpenAsync();

        await using var command = new NpgsqlCommand(
            "select current_database(), current_user, current_schema(), version();",
            connection);
        await using var reader = await command.ExecuteReaderAsync();
        if (!await reader.ReadAsync())
            return "Connected.";

        return $"Connected to {reader.GetString(0)} as {reader.GetString(1)} on schema {reader.GetString(2)}.";
    }

    public async Task<IReadOnlyList<ContainerEditorRow>> LoadContainersAsync(string connectionString)
    {
        var containersById = new Dictionary<long, ContainerEditorRow>();

        await using var connection = new NpgsqlConnection(connectionString);
        await connection.OpenAsync();

        const string sql = """
            select
                c."Id",
                c."ItemId",
                c."ItemName",
                c."RewardAmount",
                r."Id",
                r."ItemId",
                r."ItemName",
                r."MinAmount",
                r."MaxAmount",
                r."Chance",
                r."Rare",
                r."ContainerAssetId"
            from dso."Asset_Container" c
            left join dso."Asset_ContainerReward" r on r."ContainerAssetId" = c."Id"
            order by c."ItemId", r."Id";
            """;

        await using var command = new NpgsqlCommand(sql, connection);
        await using var reader = await command.ExecuteReaderAsync();
        while (await reader.ReadAsync())
        {
            var containerId = reader.GetInt64(0);
            if (!containersById.TryGetValue(containerId, out var container))
            {
                container = new ContainerEditorRow
                {
                    Id = containerId,
                    ItemId = checked((int)reader.GetInt64(1)),
                    ItemName = reader.GetString(2),
                    RewardAmount = checked((int)reader.GetInt64(3))
                };
                containersById[containerId] = container;
            }

            if (reader.IsDBNull(4))
                continue;

            var reward = new ContainerRewardEditorRow
            {
                Id = reader.GetInt64(4),
                ItemId = checked((int)reader.GetInt64(5)),
                ItemName = reader.GetString(6),
                MinAmount = checked((int)reader.GetInt64(7)),
                MaxAmount = checked((int)reader.GetInt64(8)),
                Chance = decimal.ToDouble(reader.GetDecimal(9)),
                Rare = reader.GetBoolean(10),
                ContainerAssetId = reader.GetInt64(11)
            };
            reward.MarkClean();
            container.Rewards.Add(reward);
        }

        foreach (var container in containersById.Values)
            container.MarkClean();

        return containersById.Values.OrderBy(x => x.ItemId).ToList();
    }

    public async Task SaveContainerAsync(string connectionString, ContainerEditorRow container)
    {
        await using var connection = new NpgsqlConnection(connectionString);
        await connection.OpenAsync();
        await using var transaction = await connection.BeginTransactionAsync();

        var containerId = await FindContainerIdAsync(connection, transaction, container.ItemId);
        if (containerId == 0)
        {
            const string insertContainer = """
                insert into dso."Asset_Container" ("ItemId", "ItemName", "RewardAmount")
                values (@ItemId, @ItemName, @RewardAmount)
                returning "Id";
                """;
            await using var command = new NpgsqlCommand(insertContainer, connection, transaction);
            command.Parameters.AddWithValue("ItemId", (long)container.ItemId);
            command.Parameters.AddWithValue("ItemName", LimitName(container.ItemName));
            command.Parameters.AddWithValue("RewardAmount", (long)container.RewardAmount);
            containerId = (long)(await command.ExecuteScalarAsync() ?? 0L);
        }
        else
        {
            const string updateContainer = """
                update dso."Asset_Container"
                set "ItemName" = @ItemName,
                    "RewardAmount" = @RewardAmount
                where "Id" = @Id;
                """;
            await using var command = new NpgsqlCommand(updateContainer, connection, transaction);
            command.Parameters.AddWithValue("Id", containerId);
            command.Parameters.AddWithValue("ItemName", LimitName(container.ItemName));
            command.Parameters.AddWithValue("RewardAmount", (long)container.RewardAmount);
            await command.ExecuteNonQueryAsync();
        }

        const string deleteRewards = """
            delete from dso."Asset_ContainerReward"
            where "ContainerAssetId" = @ContainerAssetId;
            """;
        await using (var command = new NpgsqlCommand(deleteRewards, connection, transaction))
        {
            command.Parameters.AddWithValue("ContainerAssetId", containerId);
            await command.ExecuteNonQueryAsync();
        }

        const string insertReward = """
            insert into dso."Asset_ContainerReward"
                ("ItemId", "ItemName", "MinAmount", "MaxAmount", "Chance", "Rare", "ContainerAssetId")
            values
                (@ItemId, @ItemName, @MinAmount, @MaxAmount, @Chance, @Rare, @ContainerAssetId)
            returning "Id";
            """;

        foreach (var reward in container.Rewards.Where(x => x.ItemId > 0))
        {
            await using var command = new NpgsqlCommand(insertReward, connection, transaction);
            command.Parameters.AddWithValue("ItemId", (long)reward.ItemId);
            command.Parameters.AddWithValue("ItemName", LimitName(reward.ItemName));
            command.Parameters.AddWithValue("MinAmount", (long)reward.MinAmount);
            command.Parameters.AddWithValue("MaxAmount", (long)reward.MaxAmount);
            command.Parameters.AddWithValue("Chance", Math.Round((decimal)reward.Chance, 2));
            command.Parameters.AddWithValue("Rare", reward.Rare);
            command.Parameters.AddWithValue("ContainerAssetId", containerId);
            reward.Id = (long)(await command.ExecuteScalarAsync() ?? 0L);
            reward.ContainerAssetId = containerId;
            reward.MarkClean();
        }

        await transaction.CommitAsync();

        container.Id = containerId;
        container.MarkClean();
        container.RefreshRewardSummary();
    }

    private static async Task<long> FindContainerIdAsync(NpgsqlConnection connection, NpgsqlTransaction transaction, int itemId)
    {
        const string sql = """
            select "Id"
            from dso."Asset_Container"
            where "ItemId" = @ItemId
            for update;
            """;

        await using var command = new NpgsqlCommand(sql, connection, transaction);
        command.Parameters.AddWithValue("ItemId", (long)itemId);
        var value = await command.ExecuteScalarAsync();
        return value is long id ? id : 0;
    }

    private static string LimitName(string value)
    {
        var trimmed = value.Trim();
        return trimmed.Length <= 100 ? trimmed : trimmed[..100];
    }

    private static string? TryBuildProductionConnectionString()
    {
        var envPath = @"C:\0-NewDmo\dmo-server-main-postgres\.env.production";
        if (!File.Exists(envPath))
            return null;

        var vars = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        foreach (var line in File.ReadAllLines(envPath))
        {
            var trimmed = line.Trim();
            if (trimmed.Length == 0 || trimmed.StartsWith('#'))
                continue;

            var index = trimmed.IndexOf('=');
            if (index <= 0)
                continue;

            vars[trimmed[..index].Trim()] = trimmed[(index + 1)..].Trim();
        }

        if (!vars.TryGetValue("POSTGRES_DB", out var database) ||
            !vars.TryGetValue("POSTGRES_USER", out var username) ||
            !vars.TryGetValue("POSTGRES_PASSWORD", out var password))
        {
            return null;
        }

        vars.TryGetValue("GAME_PUBLIC_ADDRESS", out var host);
        vars.TryGetValue("POSTGRES_PORT", out var port);
        host = string.IsNullOrWhiteSpace(host) ? "127.0.0.1" : host;
        port = string.IsNullOrWhiteSpace(port) ? "5432" : port;

        return $"Host={host};Port={port};Database={database};Username={username};Password={password};Search Path=dso;";
    }

    private sealed class EditorSettings
    {
        public string DatabaseConnection { get; set; } = string.Empty;
    }
}
