using System;
using DigitalWorldOnline.Infraestructure;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace DigitalWorldOnline.Infraestructure.Migrations
{
    [DbContext(typeof(DatabaseContext))]
    [Migration("20260523181000_AlignRequiredTemporalEndDefaults")]
    public partial class AlignRequiredTemporalEndDefaults : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                EnsureRequiredTimestampDefault(migrationBuilder, "Account_AccountBlock", "EndDate");
                EnsureRequiredTimestampDefault(migrationBuilder, "Arena_Ranking", "EndDate");
                EnsureRequiredTimestampDefault(migrationBuilder, "Character_ActiveSkill", "EndCooldown");
                EnsureRequiredTimestampDefault(migrationBuilder, "Character_ActiveSkill", "EndDate");
                EnsureRequiredTimestampDefault(migrationBuilder, "Character_Buff", "EndDate");
                EnsureRequiredTimestampDefault(migrationBuilder, "Digimon_Buff", "CoolEndDate");
                EnsureRequiredTimestampDefault(migrationBuilder, "Digimon_Buff", "EndDate");
                EnsureRequiredTimestampDefault(migrationBuilder, "Digimon_EvolutionSkill", "EndDate");
                EnsureRequiredTimestamp(migrationBuilder, "Character_Buff", "CoolEndDate");
                return;
            }

            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Account_AccountBlock", "EndDate");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Arena_Ranking", "EndDate");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Character_ActiveSkill", "EndCooldown");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Character_ActiveSkill", "EndDate");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Character_Buff", "EndDate");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Digimon_Buff", "CoolEndDate");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Digimon_Buff", "EndDate");
            EnsureRequiredTimestampDefaultMySql(migrationBuilder, "Digimon_EvolutionSkill", "EndDate");
            EnsureRequiredTimestampMySql(migrationBuilder, "Character_Buff", "CoolEndDate");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                RevertRequiredTimestampDefault(migrationBuilder, "Account_AccountBlock", "EndDate");
                RevertRequiredTimestampDefault(migrationBuilder, "Arena_Ranking", "EndDate");
                RevertRequiredTimestampDefault(migrationBuilder, "Character_ActiveSkill", "EndCooldown");
                RevertRequiredTimestampDefault(migrationBuilder, "Character_ActiveSkill", "EndDate");
                RevertRequiredTimestampDefault(migrationBuilder, "Character_Buff", "EndDate");
                RevertRequiredTimestampDefault(migrationBuilder, "Digimon_Buff", "CoolEndDate");
                RevertRequiredTimestampDefault(migrationBuilder, "Digimon_Buff", "EndDate");
                RevertRequiredTimestampDefault(migrationBuilder, "Digimon_EvolutionSkill", "EndDate");
                RevertRequiredTimestamp(migrationBuilder, "Character_Buff", "CoolEndDate");
            }
        }

        private static void EnsureRequiredTimestampDefault(MigrationBuilder migrationBuilder, string table, string column)
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

        private static void EnsureRequiredTimestamp(MigrationBuilder migrationBuilder, string table, string column)
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
        EXECUTE 'ALTER TABLE ""{table}"" ALTER COLUMN ""{column}"" SET NOT NULL';
    END IF;
END $$;");
        }

        private static void EnsureRequiredTimestampDefaultMySql(MigrationBuilder migrationBuilder, string table, string column)
        {
            migrationBuilder.Sql($@"UPDATE `{table}` SET `{column}` = CURRENT_TIMESTAMP(6) WHERE `{column}` IS NULL;");
            migrationBuilder.Sql($@"ALTER TABLE `{table}` MODIFY `{column}` datetime(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6);");
        }

        private static void EnsureRequiredTimestampMySql(MigrationBuilder migrationBuilder, string table, string column)
        {
            migrationBuilder.Sql($@"UPDATE `{table}` SET `{column}` = CURRENT_TIMESTAMP(6) WHERE `{column}` IS NULL;");
            migrationBuilder.Sql($@"ALTER TABLE `{table}` MODIFY `{column}` datetime(6) NOT NULL;");
        }

        private static void RevertRequiredTimestampDefault(MigrationBuilder migrationBuilder, string table, string column)
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

        private static void RevertRequiredTimestamp(MigrationBuilder migrationBuilder, string table, string column)
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
        EXECUTE 'ALTER TABLE ""{table}"" ALTER COLUMN ""{column}"" DROP NOT NULL';
    END IF;
END $$;");
        }
    }
}
