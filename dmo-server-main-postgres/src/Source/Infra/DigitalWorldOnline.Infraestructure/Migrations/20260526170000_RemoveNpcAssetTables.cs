using System;
using DigitalWorldOnline.Infraestructure;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace DigitalWorldOnline.Infraestructure.Migrations
{
    [DbContext(typeof(DatabaseContext))]
    [Migration("20260526170000_RemoveNpcAssetTables")]
    public partial class RemoveNpcAssetTables : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            DropTableIfExists(migrationBuilder, "Asset_NpcPortals");
            DropTableIfExists(migrationBuilder, "Asset_NpcPortalsAmount");
            DropTableIfExists(migrationBuilder, "Asset_NpcPortal");
            DropTableIfExists(migrationBuilder, "Asset_NpcItem");
            DropTableIfExists(migrationBuilder, "Asset_Npc");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Asset_Npc",
                columns: table => new
                {
                    Id = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", Npgsql.EntityFrameworkCore.PostgreSQL.Metadata.NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    NpcId = table.Column<int>(type: "integer", nullable: false),
                    MapId = table.Column<int>(type: "integer", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Asset_Npc", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "Asset_NpcItem",
                columns: table => new
                {
                    Id = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", Npgsql.EntityFrameworkCore.PostgreSQL.Metadata.NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    ItemId = table.Column<int>(type: "integer", nullable: false),
                    NpcAssetId = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Asset_NpcItem", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Asset_NpcItem_Asset_Npc_NpcAssetId",
                        column: x => x.NpcAssetId,
                        principalTable: "Asset_Npc",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Asset_NpcPortal",
                columns: table => new
                {
                    Id = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", Npgsql.EntityFrameworkCore.PostgreSQL.Metadata.NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    PortalType = table.Column<int>(type: "integer", nullable: false),
                    PortalCount = table.Column<int>(type: "integer", nullable: false),
                    NpcAssetId = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Asset_NpcPortal", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Asset_NpcPortal_Asset_Npc_NpcAssetId",
                        column: x => x.NpcAssetId,
                        principalTable: "Asset_Npc",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Asset_NpcPortalsAmount",
                columns: table => new
                {
                    Id = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", Npgsql.EntityFrameworkCore.PostgreSQL.Metadata.NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    NpcAssetId = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Asset_NpcPortalsAmount", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Asset_NpcPortalsAmount_Asset_NpcPortal_NpcAssetId",
                        column: x => x.NpcAssetId,
                        principalTable: "Asset_NpcPortal",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Asset_NpcPortals",
                columns: table => new
                {
                    Id = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", Npgsql.EntityFrameworkCore.PostgreSQL.Metadata.NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    Type = table.Column<int>(type: "integer", nullable: false),
                    ItemId = table.Column<int>(type: "integer", nullable: false),
                    ResourceAmount = table.Column<int>(type: "integer", nullable: false),
                    NpcAssetId = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Asset_NpcPortals", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Asset_NpcPortals_Asset_NpcPortalsAmount_NpcAssetId",
                        column: x => x.NpcAssetId,
                        principalTable: "Asset_NpcPortalsAmount",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(name: "IX_Asset_NpcItem_NpcAssetId", table: "Asset_NpcItem", column: "NpcAssetId");
            migrationBuilder.CreateIndex(name: "IX_Asset_NpcPortal_NpcAssetId", table: "Asset_NpcPortal", column: "NpcAssetId");
            migrationBuilder.CreateIndex(name: "IX_Asset_NpcPortals_NpcAssetId", table: "Asset_NpcPortals", column: "NpcAssetId");
            migrationBuilder.CreateIndex(name: "IX_Asset_NpcPortalsAmount_NpcAssetId", table: "Asset_NpcPortalsAmount", column: "NpcAssetId");
        }

        private static void DropTableIfExists(MigrationBuilder migrationBuilder, string table)
        {
            if (migrationBuilder.ActiveProvider.Contains("Npgsql", StringComparison.OrdinalIgnoreCase))
            {
                migrationBuilder.Sql($@"DROP TABLE IF EXISTS ""{table}"" CASCADE;");
                return;
            }

            migrationBuilder.Sql($@"DROP TABLE IF EXISTS `{table}`;");
        }
    }
}
