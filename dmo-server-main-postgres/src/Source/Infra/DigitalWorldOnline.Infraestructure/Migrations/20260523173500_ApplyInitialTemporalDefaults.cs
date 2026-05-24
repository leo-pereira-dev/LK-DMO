using System;
using DigitalWorldOnline.Infraestructure;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace DigitalWorldOnline.Infraestructure.Migrations
{
    [DbContext(typeof(DatabaseContext))]
    [Migration("20260523173500_ApplyInitialTemporalDefaults")]
    public partial class ApplyInitialTemporalDefaults : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                EnsureInitialTimestamp(migrationBuilder, "Account_Account", "CreateDate");
                EnsureInitialTimestamp(migrationBuilder, "Account_Server", "CreateDate");
                EnsureInitialTimestamp(migrationBuilder, "Account_AccountBlock", "StartDate");
                EnsureInitialTimestamp(migrationBuilder, "Arena_Competitor", "InsertDate");
                EnsureInitialTimestamp(migrationBuilder, "Arena_Ranking", "StartDate");
                EnsureInitialTimestamp(migrationBuilder, "Character_DailyPoints", "InsertDate");
                EnsureInitialTimestamp(migrationBuilder, "Character_Tamer", "CreateDate");
                EnsureInitialTimestamp(migrationBuilder, "Config_Hash", "CreatedAt");
                EnsureInitialTimestamp(migrationBuilder, "Digimon_Digimon", "CreateDate");
                EnsureInitialTimestamp(migrationBuilder, "Digimon_EvolutionMemorySkill", "AcquiredAt");
                EnsureInitialTimestamp(migrationBuilder, "Event_TimeReward", "StartTime");
                EnsureInitialTimestamp(migrationBuilder, "Guild_Guild", "CreationDate");
                EnsureInitialTimestamp(migrationBuilder, "Guild_Historic", "Date");
                EnsureInitialTimestamp(migrationBuilder, "Routine_Routine", "CreatedAt");
                EnsureInitialTimestamp(migrationBuilder, "Security_ChatMessage", "Time");
                EnsureInitialTimestamp(migrationBuilder, "Security_LoginTry", "Date");
                EnsureInitialTimestamp(migrationBuilder, "Shared_ItemStorageAccount", "CreatedAt");
                EnsureInitialTimestamp(migrationBuilder, "Shared_ItemStorageCharacter", "CreatedAt");
                EnsureInitialTimestamp(migrationBuilder, "Shared_ItemStorageInstance", "CreatedAt");
                return;
            }

            EnsureInitialTimestampMySql(migrationBuilder, "Account_Account", "CreateDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Account_Server", "CreateDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Account_AccountBlock", "StartDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Arena_Competitor", "InsertDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Arena_Ranking", "StartDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Character_DailyPoints", "InsertDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Character_Tamer", "CreateDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Config_Hash", "CreatedAt");
            EnsureInitialTimestampMySql(migrationBuilder, "Digimon_Digimon", "CreateDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Digimon_EvolutionMemorySkill", "AcquiredAt");
            EnsureInitialTimestampMySql(migrationBuilder, "Event_TimeReward", "StartTime");
            EnsureInitialTimestampMySql(migrationBuilder, "Guild_Guild", "CreationDate");
            EnsureInitialTimestampMySql(migrationBuilder, "Guild_Historic", "Date");
            EnsureInitialTimestampMySql(migrationBuilder, "Routine_Routine", "CreatedAt");
            EnsureInitialTimestampMySql(migrationBuilder, "Security_ChatMessage", "Time");
            EnsureInitialTimestampMySql(migrationBuilder, "Security_LoginTry", "Date");
            EnsureInitialTimestampMySql(migrationBuilder, "Shared_ItemStorageAccount", "CreatedAt");
            EnsureInitialTimestampMySql(migrationBuilder, "Shared_ItemStorageCharacter", "CreatedAt");
            EnsureInitialTimestampMySql(migrationBuilder, "Shared_ItemStorageInstance", "CreatedAt");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                RevertInitialTimestamp(migrationBuilder, "Account_Account", "CreateDate");
                RevertInitialTimestamp(migrationBuilder, "Account_Server", "CreateDate");
                RevertInitialTimestamp(migrationBuilder, "Account_AccountBlock", "StartDate");
                RevertInitialTimestamp(migrationBuilder, "Arena_Competitor", "InsertDate");
                RevertInitialTimestamp(migrationBuilder, "Arena_Ranking", "StartDate");
                RevertInitialTimestamp(migrationBuilder, "Character_DailyPoints", "InsertDate");
                RevertInitialTimestamp(migrationBuilder, "Character_Tamer", "CreateDate");
                RevertInitialTimestamp(migrationBuilder, "Config_Hash", "CreatedAt");
                RevertInitialTimestamp(migrationBuilder, "Digimon_Digimon", "CreateDate");
                RevertInitialTimestamp(migrationBuilder, "Digimon_EvolutionMemorySkill", "AcquiredAt");
                RevertInitialTimestamp(migrationBuilder, "Event_TimeReward", "StartTime");
                RevertInitialTimestamp(migrationBuilder, "Guild_Guild", "CreationDate");
                RevertInitialTimestamp(migrationBuilder, "Guild_Historic", "Date");
                RevertInitialTimestamp(migrationBuilder, "Routine_Routine", "CreatedAt");
                RevertInitialTimestamp(migrationBuilder, "Security_ChatMessage", "Time");
                RevertInitialTimestamp(migrationBuilder, "Security_LoginTry", "Date");
                RevertInitialTimestamp(migrationBuilder, "Shared_ItemStorageAccount", "CreatedAt");
                RevertInitialTimestamp(migrationBuilder, "Shared_ItemStorageCharacter", "CreatedAt");
                RevertInitialTimestamp(migrationBuilder, "Shared_ItemStorageInstance", "CreatedAt");
            }
        }

        private static void EnsureInitialTimestamp(MigrationBuilder migrationBuilder, string table, string column)
        {
            migrationBuilder.Sql($@"
DO $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM information_schema.columns
        WHERE table_schema = current_schema()
          AND table_name = '{table}'
          AND column_name = '{column}'
    ) THEN
        EXECUTE 'UPDATE ""{table}"" SET ""{column}"" = CURRENT_TIMESTAMP WHERE ""{column}"" IS NULL';
        EXECUTE 'ALTER TABLE ""{table}"" ALTER COLUMN ""{column}"" SET DEFAULT CURRENT_TIMESTAMP';
        EXECUTE 'ALTER TABLE ""{table}"" ALTER COLUMN ""{column}"" SET NOT NULL';
    END IF;
END $$;");
        }

        private static void EnsureInitialTimestampMySql(MigrationBuilder migrationBuilder, string table, string column)
        {
            migrationBuilder.Sql($@"UPDATE `{table}` SET `{column}` = CURRENT_TIMESTAMP(6) WHERE `{column}` IS NULL;");
            migrationBuilder.Sql($@"ALTER TABLE `{table}` MODIFY `{column}` datetime(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6);");
        }

        private static void RevertInitialTimestamp(MigrationBuilder migrationBuilder, string table, string column)
        {
            migrationBuilder.Sql($@"
DO $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM information_schema.columns
        WHERE table_schema = current_schema()
          AND table_name = '{table}'
          AND column_name = '{column}'
    ) THEN
        EXECUTE 'ALTER TABLE ""{table}"" ALTER COLUMN ""{column}"" DROP DEFAULT';
        EXECUTE 'ALTER TABLE ""{table}"" ALTER COLUMN ""{column}"" DROP NOT NULL';
    END IF;
END $$;");
        }
    }
}
