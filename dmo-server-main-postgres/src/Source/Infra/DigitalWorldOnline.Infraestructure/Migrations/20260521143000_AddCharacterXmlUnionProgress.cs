using System;
using DigitalWorldOnline.Infraestructure;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace DigitalWorldOnline.Infraestructure.Migrations
{
    [DbContext(typeof(DatabaseContext))]
    [Migration("20260521143000_AddCharacterXmlUnionProgress")]
    public partial class AddCharacterXmlUnionProgress : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                migrationBuilder.CreateTable(
                    name: "Character_XmlUnionProgress",
                    columns: table => new
                    {
                        Id = table.Column<long>(type: "bigint", nullable: false)
                            .Annotation("Npgsql:ValueGenerationStrategy", Npgsql.EntityFrameworkCore.PostgreSQL.Metadata.NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                        Level = table.Column<short>(type: "smallint", nullable: false, defaultValue: (short)0),
                        CurrentExperience = table.Column<int>(type: "integer", nullable: false, defaultValue: 0),
                        RequiredExperience = table.Column<int>(type: "integer", nullable: false, defaultValue: 10000),
                        ClaimedRewardMask = table.Column<int>(type: "integer", nullable: false, defaultValue: 0),
                        CreatedAt = table.Column<DateTime>(type: "timestamp without time zone", nullable: false, defaultValueSql: "CURRENT_TIMESTAMP"),
                        UpdatedAt = table.Column<DateTime>(type: "timestamp without time zone", nullable: false, defaultValueSql: "CURRENT_TIMESTAMP"),
                        CharacterId = table.Column<long>(type: "bigint", nullable: false)
                    },
                    constraints: table =>
                    {
                        table.PrimaryKey("PK_Character_XmlUnionProgress", x => x.Id);
                        table.ForeignKey(
                            name: "FK_Character_XmlUnionProgress_Character_Tamer_CharacterId",
                            column: x => x.CharacterId,
                            principalTable: "Character_Tamer",
                            principalColumn: "Id",
                            onDelete: ReferentialAction.Cascade);
                    });

                migrationBuilder.CreateIndex(
                    name: "IX_Character_XmlUnionProgress_CharacterId",
                    table: "Character_XmlUnionProgress",
                    column: "CharacterId",
                    unique: true);

                migrationBuilder.Sql(@"
INSERT INTO ""Character_XmlUnionProgress""
(""Level"", ""CurrentExperience"", ""RequiredExperience"", ""ClaimedRewardMask"", ""CreatedAt"", ""UpdatedAt"", ""CharacterId"")
SELECT 0, 0, 10000, 0, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, c.""Id""
FROM ""Character_Tamer"" c
LEFT JOIN ""Character_XmlUnionProgress"" x ON x.""CharacterId"" = c.""Id""
WHERE x.""Id"" IS NULL;");

                return;
            }

            migrationBuilder.CreateTable(
                name: "Character_XmlUnionProgress",
                columns: table => new
                {
                    Id = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("MySql:ValueGenerationStrategy", MySqlValueGenerationStrategy.IdentityColumn),
                    Level = table.Column<byte>(type: "tinyint unsigned", nullable: false, defaultValue: (byte)0),
                    CurrentExperience = table.Column<int>(type: "int", nullable: false, defaultValue: 0),
                    RequiredExperience = table.Column<int>(type: "int", nullable: false, defaultValue: 10000),
                    ClaimedRewardMask = table.Column<ushort>(type: "smallint unsigned", nullable: false, defaultValue: (ushort)0),
                    CreatedAt = table.Column<DateTime>(type: "datetime(6)", nullable: false, defaultValueSql: "CURRENT_TIMESTAMP(6)"),
                    UpdatedAt = table.Column<DateTime>(type: "datetime(6)", nullable: false, defaultValueSql: "CURRENT_TIMESTAMP(6)"),
                    CharacterId = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Character_XmlUnionProgress", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Character_XmlUnionProgress_Character_Tamer_CharacterId",
                        column: x => x.CharacterId,
                        principalTable: "Character_Tamer",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                })
                .Annotation("MySql:CharSet", "utf8mb4");

            migrationBuilder.CreateIndex(
                name: "IX_Character_XmlUnionProgress_CharacterId",
                table: "Character_XmlUnionProgress",
                column: "CharacterId",
                unique: true);

            migrationBuilder.Sql(@"
INSERT INTO `Character_XmlUnionProgress`
(`Level`, `CurrentExperience`, `RequiredExperience`, `ClaimedRewardMask`, `CreatedAt`, `UpdatedAt`, `CharacterId`)
SELECT 0, 0, 10000, 0, CURRENT_TIMESTAMP(6), CURRENT_TIMESTAMP(6), c.`Id`
FROM `Character_Tamer` c
LEFT JOIN `Character_XmlUnionProgress` x ON x.`CharacterId` = c.`Id`
WHERE x.`Id` IS NULL;");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(name: "Character_XmlUnionProgress");
        }
    }
}
