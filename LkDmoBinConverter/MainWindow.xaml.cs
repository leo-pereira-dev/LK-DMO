using System.IO;
using System.Windows;
using Forms = System.Windows.Forms;
using LkDmoBinConverter.Models;
using LkDmoBinConverter.Services;

namespace LkDmoBinConverter;

public partial class MainWindow : Window
{
    private readonly ItemListCraftConverter _craftConverter = new();
    private readonly NpcType25Converter _npcType25Converter = new();
    private readonly MapListConverter _mapListConverter = new();
    private readonly MonsterConverter _monsterConverter = new();

    public MainWindow()
    {
        InitializeComponent();
        GdmoPathBox.Text = @"C:\0-NewDmo\Dcommerce\Bin";
        LkdmoPathBox.Text = @"C:\0-NewDmo\ClientDist1\Data\Pack03";
        OutputPathBox.Text = @"C:\0-NewDmo\build-temp\LkDmoBinConverter";
        ReportBox.Text = "Choose paths and run Analyze / Dry Run. Pack03 is only modified when Apply to Pack03 is used.";
    }

    private void BrowseGdmo_Click(object sender, RoutedEventArgs e) => BrowseFolderInto(GdmoPathBox);
    private void BrowseOutput_Click(object sender, RoutedEventArgs e) => BrowseFolderInto(OutputPathBox);

    private void BrowseLkdmo_Click(object sender, RoutedEventArgs e)
    {
        using var dialog = new Forms.OpenFileDialog
        {
            Title = "Select supported bin, Pack03.pf, or Pack03.hf",
            Filter = "Supported files|ItemList.bin;Npc.bin;MapList.bin;Monster.bin;MapMonsterList.bin;Pack03.pf;Pack03.hf|All files|*.*",
            CheckFileExists = true
        };

        if (dialog.ShowDialog() == Forms.DialogResult.OK)
        {
            if (Path.GetFileName(dialog.FileName).Equals("Pack03.pf", StringComparison.OrdinalIgnoreCase) ||
                Path.GetFileName(dialog.FileName).Equals("Pack03.hf", StringComparison.OrdinalIgnoreCase))
            {
                LkdmoPathBox.Text = Path.Combine(Path.GetDirectoryName(dialog.FileName)!, "Pack03");
            }
            else
            {
                LkdmoPathBox.Text = dialog.FileName;
            }
        }
    }

    private static void BrowseFolderInto(System.Windows.Controls.TextBox box)
    {
        using var dialog = new Forms.FolderBrowserDialog
        {
            Description = "Select folder",
            UseDescriptionForTitle = true,
            ShowNewFolderButton = true,
            SelectedPath = Directory.Exists(box.Text) ? box.Text : @"C:\0-NewDmo"
        };

        if (dialog.ShowDialog() == Forms.DialogResult.OK)
            box.Text = dialog.SelectedPath;
    }

    private async void Analyze_Click(object sender, RoutedEventArgs e) => await RunAsync(apply: false);

    private async void Apply_Click(object sender, RoutedEventArgs e)
    {
        var craftMode = ConversionModeBox.SelectedIndex == 0;
        var mapMode = ConversionModeBox.SelectedIndex == 2;
        var monsterMode = ConversionModeBox.SelectedIndex == 3;
        var safeCraft = craftMode && PatchEmbeddedItemListCraftBox.IsChecked != true;
        var message = safeCraft
            ? "This will patch Pack03 sidecar craft bins only and create .bak files first. The embedded ItemList craft section will stay unchanged in safe mode. Close the game client before continuing. Apply now?"
            : mapMode
                ? "This will patch Pack03 MapList.bin and create .bak files first. Close the game client before continuing. Apply now?"
                : monsterMode
                    ? "This will patch Pack03 Monster.bin and MapMonsterList.bin and create .bak files first. Close the game client before continuing. Apply now?"
                    : "This will patch Pack03 and create .bak files first. Close the game client before continuing. Apply now?";

        var result = System.Windows.MessageBox.Show(
            message,
            "Apply to Pack03",
            MessageBoxButton.YesNo,
            MessageBoxImage.Warning);

        if (result == MessageBoxResult.Yes)
            await RunAsync(apply: true);
    }

    private async Task RunAsync(bool apply)
    {
        SetBusy(true);
        ReportBox.Text = "Working...";
        var npcMode = ConversionModeBox.SelectedIndex == 1;
        var mapMode = ConversionModeBox.SelectedIndex == 2;
        var monsterMode = ConversionModeBox.SelectedIndex == 3;
        StatusText.Text = apply ? "Applying conversion to Pack03..." : "Running dry-run conversion...";

        try
        {
            var options = new CraftConversionOptions
            {
                GdmoBinPath = GdmoPathBox.Text.Trim(),
                LkdmoInputPath = LkdmoPathBox.Text.Trim(),
                OutputDirectory = OutputPathBox.Text.Trim(),
                PatchEmbeddedItemListCraft = PatchEmbeddedItemListCraftBox.IsChecked == true
            };

            if (monsterMode)
            {
                var report = await _monsterConverter.ConvertMonstersAsync(options, apply, CancellationToken.None);
                ReportBox.Text = report.ToDetailedText();
                StatusText.Text = report.Success ? "Completed." : "Completed with warnings/errors.";
            }
            else if (mapMode)
            {
                var report = await _mapListConverter.ConvertMapListAsync(options, apply, CancellationToken.None);
                ReportBox.Text = report.ToDetailedText();
                StatusText.Text = report.Success ? "Completed." : "Completed with warnings/errors.";
            }
            else if (npcMode)
            {
                var report = await _npcType25Converter.ConvertType25Async(options, apply, CancellationToken.None);
                ReportBox.Text = report.ToDetailedText();
                StatusText.Text = report.Success ? "Completed." : "Completed with warnings/errors.";
            }
            else
            {
                var report = await _craftConverter.ConvertCraftAsync(options, apply, CancellationToken.None);
                ReportBox.Text = report.ToDetailedText();
                StatusText.Text = report.Success ? "Completed." : "Completed with warnings/errors.";
            }
        }
        catch (Exception ex)
        {
            ReportBox.Text = ex.ToString();
            StatusText.Text = "Failed.";
        }
        finally
        {
            SetBusy(false);
        }
    }

    private void SetBusy(bool busy)
    {
        AnalyzeButton.IsEnabled = !busy;
        ApplyButton.IsEnabled = !busy;
    }
}
