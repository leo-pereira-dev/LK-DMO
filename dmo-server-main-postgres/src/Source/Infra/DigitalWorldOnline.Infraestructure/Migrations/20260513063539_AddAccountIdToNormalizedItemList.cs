using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace DigitalWorldOnline.Infraestructure.Migrations
{
    public partial class AddAccountIdToNormalizedItemList : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<long>(
                name: "AccountId",
                table: "Shared_ItemListNormalized",
                type: "bigint",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Shared_ItemListNormalized_Account_Type",
                table: "Shared_ItemListNormalized",
                columns: new[] { "AccountId", "Type" });
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropIndex(
                name: "IX_Shared_ItemListNormalized_Account_Type",
                table: "Shared_ItemListNormalized");

            migrationBuilder.DropColumn(
                name: "AccountId",
                table: "Shared_ItemListNormalized");
        }
    }
}
