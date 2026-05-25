using System;
using DigitalWorldOnline.Infraestructure;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace DigitalWorldOnline.Infraestructure.Migrations
{
    [DbContext(typeof(DatabaseContext))]
    [Migration("20260525120000_AddCharacterEncyclopediaDeckId")]
    public partial class AddCharacterEncyclopediaDeckId : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                migrationBuilder.Sql(@"
ALTER TABLE ""Character_Tamer""
ADD COLUMN IF NOT EXISTS ""EncyclopediaDeckId"" integer NOT NULL DEFAULT 0;");
                return;
            }

            migrationBuilder.Sql(@"
ALTER TABLE `Character_Tamer`
ADD COLUMN `EncyclopediaDeckId` int NOT NULL DEFAULT 0;");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                migrationBuilder.Sql(@"
ALTER TABLE ""Character_Tamer""
DROP COLUMN IF EXISTS ""EncyclopediaDeckId"";");
                return;
            }

            migrationBuilder.DropColumn(
                name: "EncyclopediaDeckId",
                table: "Character_Tamer");
        }
    }
}
