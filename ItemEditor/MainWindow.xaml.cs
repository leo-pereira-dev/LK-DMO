using System.ComponentModel;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Shapes;
using Forms = System.Windows.Forms;
using ItemEditor.Models;
using ItemEditor.Services;
using MediaBrush = System.Windows.Media.Brush;
using MediaBrushes = System.Windows.Media.Brushes;
using MediaColor = System.Windows.Media.Color;
using MediaColorConverter = System.Windows.Media.ColorConverter;
using WpfImage = System.Windows.Controls.Image;
using WpfPoint = System.Windows.Point;
using WpfRectangle = System.Windows.Shapes.Rectangle;

namespace ItemEditor;

public partial class MainWindow : Window
{
    private readonly record struct MapCanvasLayout(double Left, double Top, double Size);

    private readonly ItemBinWorkspace _workspace = new();
    private readonly AssetCatalogWorkspace _assets = new();
    private readonly DatabaseContainerService _database = new();
    private readonly MapPreviewService _mapPreview = new();
    private ICollectionView? _view;
    private ICollectionView? _npcView;
    private ICollectionView? _craftView;
    private ICollectionView? _shopView;
    private ICollectionView? _questView;
    private bool _syncingKind;
    private bool _refreshingQuestGoalContext;
    private readonly ObservableCollection<LookupOption> _npcMapFilterOptions = [];

    public IReadOnlyList<EffectOption> ApplyOptions => SkillEffectCatalog.ApplyOptions;
    public IReadOnlyList<EffectOption> MethodOptions => SkillEffectCatalog.MethodOptions;
    public IReadOnlyList<ItemKindOption> ItemKindOptions => ItemKindCatalog.Options;
    public IReadOnlyList<LookupOption> NpcTypeOptions => NpcTypeCatalog.Options;
    public IReadOnlyList<LookupOption> NpcTypeFilterOptions => NpcTypeCatalog.FilterOptions;
    public IReadOnlyList<LookupOption> ShopTypeFilterOptions => NpcTypeCatalog.ShopFilterOptions;
    public IReadOnlyList<LookupOption> QuestTypeOptions => QuestTypeCatalog.Options;
    public IReadOnlyList<LookupOption> QuestTypeFilterOptions => QuestTypeCatalog.FilterOptions;
    public IReadOnlyList<LookupOption> QuestStartTargetOptions => QuestTargetCatalog.StartTargetOptions;
    public IReadOnlyList<LookupOption> QuestCompletionTargetOptions => QuestTargetCatalog.CompletionTargetOptions;
    public IReadOnlyList<LookupOption> QuestSupplyTypeOptions => QuestSupplyCatalog.Options;
    public IReadOnlyList<LookupOption> QuestPreconditionOptions => QuestPreconditionCatalog.Options;
    public IReadOnlyList<LookupOption> QuestGoalOptions => QuestGoalCatalog.Options;
    public IReadOnlyList<LookupOption> QuestRewardMethodOptions => QuestRewardCatalog.MethodOptions;
    public IReadOnlyList<LookupOption> QuestRewardTypeOptions => QuestRewardCatalog.TypeOptions;

    public MainWindow()
    {
        InitializeComponent();
        ItemKindComboBox.ItemsSource = ItemKindOptions;
        NpcMapFilterComboBox.ItemsSource = _npcMapFilterOptions;
        CraftMapFilterComboBox.ItemsSource = _npcMapFilterOptions;
        ShopMapFilterComboBox.ItemsSource = _npcMapFilterOptions;
        NpcTypeFilterComboBox.ItemsSource = NpcTypeFilterOptions;
        NpcTypeFilterComboBox.SelectedIndex = 0;
        ShopTypeFilterComboBox.ItemsSource = ShopTypeFilterOptions;
        ShopTypeFilterComboBox.SelectedIndex = 0;
        QuestTypeFilterComboBox.ItemsSource = QuestTypeFilterOptions;
        QuestTypeFilterComboBox.SelectedIndex = 0;
        SearchBox.Text = string.Empty;
        RewardItemSearchBox.Text = string.Empty;
        SkillSearchBox.Text = string.Empty;
        DbConnectionBox.Text = _database.LoadConnectionString();
        SetDbStatus("Database not tested.", "Muted");
        var defaultPath = @"C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english";
        if (Directory.Exists(defaultPath))
            LoadBins(defaultPath);
    }

    private void SetDbStatus(string message, string brushKey)
    {
        DbStatusText.Text = message;
        if (TryFindResource(brushKey) is MediaBrush brush)
            DbStatusText.Foreground = brush;
    }

    private void SetDbControlsEnabled(bool enabled)
    {
        TestDbButton.IsEnabled = enabled;
        LoadDbBoxesButton.IsEnabled = enabled;
        SaveBoxDbButton.IsEnabled = enabled;
    }

    private void LoadBins_Click(object sender, RoutedEventArgs e)
    {
        using var dialog = new Forms.FolderBrowserDialog
        {
            Description = "Select folder containing ItemList.bin, Skill.bin, Buff.bin, Container.bin",
            UseDescriptionForTitle = true,
            SelectedPath = Directory.Exists(_workspace.BinDirectory)
                ? _workspace.BinDirectory
                : @"C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english"
        };

        if (dialog.ShowDialog() == Forms.DialogResult.OK)
            LoadBins(dialog.SelectedPath);
    }

    private void LoadPack_Click(object sender, RoutedEventArgs e)
    {
        using var dialog = new Forms.OpenFileDialog
        {
            Title = "Select Pack01.pf, Pack01.hf, Pack03.pf, or Pack03.hf",
            Filter = "Pack files|*.pf;*.hf|All files|*.*",
            CheckFileExists = true,
            InitialDirectory = @"C:\0-NewDmo\NãoUsarEsseClient\Data"
        };

        if (dialog.ShowDialog() == Forms.DialogResult.OK)
            LoadBins(dialog.FileName);
    }

    private void LoadBins(string path)
    {
        try
        {
            _workspace.Load(path);
            _mapPreview.Configure(path);
            ItemListBox.ItemsSource = _workspace.Items;
            _view = CollectionViewSource.GetDefaultView(ItemListBox.ItemsSource);
            _view.Filter = FilterItem;
            _view.SortDescriptions.Clear();
            _view.SortDescriptions.Add(new SortDescription(nameof(ItemEditorRow.ItemId), ListSortDirection.Ascending));
            _view.SortDescriptions.Add(new SortDescription(nameof(ItemEditorRow.Name), ListSortDirection.Ascending));
            SourceText.Text = path;
            _assets.Load(path, _workspace.BinDirectory, _workspace);
            BindAssetTabs();
            StatusText.Text = $"{_workspace.StatusSummary} {_assets.StatusSummary}";
            RefreshRewardItemPicker();
            RefreshSkillPicker();
            if (_workspace.Items.Count > 0)
                ItemListBox.SelectedIndex = 0;
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "Load failed", MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText.Text = "Load failed.";
        }
    }

    private void BindAssetTabs()
    {
        RefreshNpcMapFilterOptions();
        _npcView = CreateNpcView(FilterNpc);
        NpcListBox.ItemsSource = _npcView;
        _craftView = CreateNpcView(FilterCraft);
        CraftListBox.ItemsSource = _craftView;
        _shopView = CreateNpcView(FilterShop);
        ShopListBox.ItemsSource = _shopView;

        QuestListBox.ItemsSource = _assets.Quests;
        _questView = CollectionViewSource.GetDefaultView(QuestListBox.ItemsSource);
        _questView.Filter = FilterQuest;
        _questView.SortDescriptions.Clear();
        _questView.SortDescriptions.Add(new SortDescription(nameof(QuestEditorRow.QuestId), ListSortDirection.Ascending));

        if (_assets.Npcs.Count > 0)
            NpcListBox.SelectedIndex = 0;
        if (_assets.Quests.Count > 0)
            QuestListBox.SelectedIndex = 0;
        if (CraftListBox.Items.Count > 0)
            CraftListBox.SelectedIndex = 0;
        if (ShopListBox.Items.Count > 0)
            ShopListBox.SelectedIndex = 0;
    }

    private ICollectionView CreateNpcView(Predicate<object> filter)
    {
        var source = new CollectionViewSource { Source = _assets.Npcs };
        var view = source.View;
        view.Filter = filter;
        view.SortDescriptions.Clear();
        view.SortDescriptions.Add(new SortDescription(nameof(NpcEditorRow.MapId), ListSortDirection.Ascending));
        view.SortDescriptions.Add(new SortDescription(nameof(NpcEditorRow.NpcId), ListSortDirection.Ascending));
        return view;
    }

    private void RefreshNpcMapFilterOptions()
    {
        var selectedMap = (NpcMapFilterComboBox.SelectedItem as LookupOption)?.Id ?? -1;
        var selectedCraftMap = (CraftMapFilterComboBox.SelectedItem as LookupOption)?.Id ?? -1;
        var selectedShopMap = (ShopMapFilterComboBox.SelectedItem as LookupOption)?.Id ?? -1;
        _npcMapFilterOptions.Clear();
        _npcMapFilterOptions.Add(new LookupOption(-1, "All maps"));
        foreach (var mapId in _assets.Npcs.Select(x => x.MapId).Distinct().OrderBy(x => x))
            _npcMapFilterOptions.Add(new LookupOption(mapId, _assets.MapName(mapId)));

        NpcMapFilterComboBox.SelectedItem = _npcMapFilterOptions.FirstOrDefault(x => x.Id == selectedMap)
                                            ?? _npcMapFilterOptions.FirstOrDefault();
        CraftMapFilterComboBox.SelectedItem = _npcMapFilterOptions.FirstOrDefault(x => x.Id == selectedCraftMap)
                                              ?? _npcMapFilterOptions.FirstOrDefault();
        ShopMapFilterComboBox.SelectedItem = _npcMapFilterOptions.FirstOrDefault(x => x.Id == selectedShopMap)
                                             ?? _npcMapFilterOptions.FirstOrDefault();
    }

    private bool FilterItem(object obj)
    {
        if (obj is not ItemEditorRow item)
            return false;

        var filter = SearchBox.Text.Trim();
        if (filter.Length == 0)
            return true;

        return item.ItemId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               item.Name.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               item.Description.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               item.TypeComment.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               item.TypeDisplay.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               item.SkillCode.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               item.EffectKind.Contains(filter, StringComparison.OrdinalIgnoreCase);
    }

    private void SearchBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        => _view?.Refresh();

    private bool FilterNpc(object obj)
    {
        if (obj is not NpcEditorRow npc)
            return false;

        if (NpcTypeFilterComboBox?.SelectedItem is LookupOption type && type.Id >= 0 && npc.TypeId != type.Id)
            return false;

        if (NpcMapFilterComboBox?.SelectedItem is LookupOption map && map.Id >= 0 && npc.MapId != map.Id)
            return false;

        var filter = NpcSearchBox?.Text.Trim() ?? string.Empty;
        return MatchesNpcSearch(npc, filter);
    }

    private bool FilterCraft(object obj)
    {
        if (obj is not NpcEditorRow npc || (!NpcTypeCatalog.IsCraft(npc.TypeId) && npc.CraftRecipes.Count == 0))
            return false;

        if (CraftMapFilterComboBox?.SelectedItem is LookupOption map && map.Id >= 0 && npc.MapId != map.Id)
            return false;

        return MatchesNpcSearch(npc, CraftSearchBox?.Text.Trim() ?? string.Empty);
    }

    private bool FilterShop(object obj)
    {
        if (obj is not NpcEditorRow npc || !NpcTypeCatalog.IsShop(npc.TypeId))
            return false;

        if (ShopTypeFilterComboBox?.SelectedItem is LookupOption type && type.Id >= 0 && npc.TypeId != type.Id)
            return false;

        if (ShopMapFilterComboBox?.SelectedItem is LookupOption map && map.Id >= 0 && npc.MapId != map.Id)
            return false;

        return MatchesNpcSearch(npc, ShopSearchBox?.Text.Trim() ?? string.Empty);
    }

    private static bool MatchesNpcSearch(NpcEditorRow npc, string filter)
    {
        if (filter.Length == 0)
            return true;

        return npc.NpcId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               npc.Name.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               npc.PenName.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               npc.Talk.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               npc.TypeName.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               npc.ModelId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               npc.Items.Any(x => x.ItemId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
                                  x.ItemName.Contains(filter, StringComparison.OrdinalIgnoreCase)) ||
               npc.CraftRecipes.Any(x => x.SequencialId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
                                          x.ItemId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
                                          x.ItemName.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
                                          x.MaterialsSummary.Contains(filter, StringComparison.OrdinalIgnoreCase)) ||
               npc.QuestLinks.Any(x => x.QuestId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
                                       x.QuestTitle.Contains(filter, StringComparison.OrdinalIgnoreCase));
    }

    private bool FilterQuest(object obj)
    {
        if (obj is not QuestEditorRow quest)
            return false;

        if (QuestTypeFilterComboBox?.SelectedItem is LookupOption type && type.Id >= 0 && quest.QuestType != type.Id)
            return false;

        var targetFilter = QuestTargetFilterBox?.Text.Trim() ?? string.Empty;
        if (targetFilter.Length > 0 &&
            !quest.StartTargetId.ToString().Contains(targetFilter, StringComparison.OrdinalIgnoreCase) &&
            !quest.TargetValue.ToString().Contains(targetFilter, StringComparison.OrdinalIgnoreCase) &&
            !quest.StartNpcName.Contains(targetFilter, StringComparison.OrdinalIgnoreCase) &&
            !quest.TargetNpcName.Contains(targetFilter, StringComparison.OrdinalIgnoreCase))
            return false;

        var filter = QuestSearchBox?.Text.Trim() ?? string.Empty;
        if (filter.Length == 0)
            return true;

        return quest.QuestId.ToString().Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               quest.Title.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               quest.TitleTab.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               quest.Simple.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               quest.Body.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               quest.QuestTypeName.Contains(filter, StringComparison.OrdinalIgnoreCase) ||
               quest.Goals.Any(x => x.Summary.Contains(filter, StringComparison.OrdinalIgnoreCase)) ||
               quest.Rewards.Any(x => x.Summary.Contains(filter, StringComparison.OrdinalIgnoreCase)) ||
               quest.Supplies.Any(x => x.DisplayName.Contains(filter, StringComparison.OrdinalIgnoreCase));
    }

    private void NpcFilter_Changed(object sender, RoutedEventArgs e)
        => _npcView?.Refresh();

    private void CraftFilter_Changed(object sender, RoutedEventArgs e)
        => _craftView?.Refresh();

    private void ShopFilter_Changed(object sender, RoutedEventArgs e)
        => _shopView?.Refresh();

    private void NpcListBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (NpcListBox.SelectedItem is NpcEditorRow npc)
        {
            StatusText.Text = $"{npc.DisplayName} | {npc.AssetSummary}";
            RenderNpcMinimap();
        }
    }

    private void NpcMapCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        => RenderNpcMinimap();

    private void NpcPositionField_LostFocus(object sender, RoutedEventArgs e)
        => RenderNpcMinimap();

    private void SaveNpcPositions_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            var message = _assets.SaveNpcPositions();
            StatusText.Text = message;
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.ToString(), "Save NPC positions failed", MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText.Text = "Save NPC positions failed.";
        }
    }

    private void NpcMapCanvas_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
    {
        if (NpcListBox.SelectedItem is not NpcEditorRow npc)
        {
            StatusText.Text = "Select an NPC before placing it on the map.";
            return;
        }

        if (npc.MapId <= 0)
        {
            StatusText.Text = "Set the NPC map id before placing it on the minimap.";
            return;
        }

        var position = e.GetPosition(NpcMapCanvas);
        var layout = GetMapCanvasLayout(NpcMapCanvas.ActualWidth, NpcMapCanvas.ActualHeight);
        if (!IsInsideMapLayout(position.X, position.Y, layout))
        {
            StatusText.Text = "Click inside the visible map square to place the NPC.";
            return;
        }

        var mapSize = _assets.GetMapSize(npc.MapId);
        var world = CanvasToMap(position.X, position.Y, layout, npc.MapId, mapSize.Width, mapSize.Height);
        _assets.SetNpcMapPosition(npc, world.X, world.Y);
        RenderNpcMinimap();
        StatusText.Text = $"Moved {npc.DisplayName} to X {npc.MapPositionX}, Y {npc.MapPositionY} on {npc.MapDisplay}.";
    }

    private void RenderNpcMinimap()
    {
        if (!IsLoaded || NpcMapCanvas == null)
            return;

        NpcMapCanvas.Children.Clear();
        var width = Math.Max(1, NpcMapCanvas.ActualWidth);
        var height = Math.Max(1, NpcMapCanvas.ActualHeight);

        if (NpcListBox.SelectedItem is not NpcEditorRow selected || selected.MapId <= 0)
        {
            AddCanvasLabel("Select an NPC with a map id to preview positions.", 12, 12, "#9AA8B8");
            return;
        }

        var mapNpcs = _assets.NpcsOnMap(selected.MapId);
        var preview = _mapPreview.ResolveMapImage(_assets.MapTextureCandidates(selected.MapId));
        var layout = GetMapCanvasLayout(width, height);
        var mapSize = _assets.GetMapSize(selected.MapId);
        DrawMinimapBackground(width, height, layout, preview.Image, preview.Status);
        DrawMinimapGrid(layout, preview.Image != null);
        AddCanvasLabel($"{selected.MapDisplay} | {mapNpcs.Count:N0} positioned NPCs", layout.Left + 12, layout.Top + 10, "#EDF5FF");

        foreach (var npc in mapNpcs)
            DrawNpcMarker(npc, selected, layout, selected.MapId, mapSize.Width, mapSize.Height);

        if (!selected.HasMapPosition)
            AddCanvasLabel("Selected NPC has no MapNpc.bin position. Click the visible map square to place it.", layout.Left + 12, layout.Top + layout.Size - 30, "#F0B35A");
    }

    private void DrawMinimapBackground(double width, double height, MapCanvasLayout layout, ImageSource? image, string status)
    {
        var canvasBackground = new WpfRectangle
        {
            Width = width,
            Height = height,
            Fill = new LinearGradientBrush(
                MediaColor.FromRgb(5, 13, 20),
                MediaColor.FromRgb(2, 6, 10),
                35),
            IsHitTestVisible = false
        };
        NpcMapCanvas.Children.Add(canvasBackground);

        if (image != null)
        {
            var mapImage = new WpfImage
            {
                Source = image,
                Width = layout.Size,
                Height = layout.Size,
                Stretch = Stretch.Fill,
                Opacity = 0.92,
                IsHitTestVisible = false
            };
            Canvas.SetLeft(mapImage, layout.Left);
            Canvas.SetTop(mapImage, layout.Top);
            NpcMapCanvas.Children.Add(mapImage);
        }
        else
        {
            var fallback = new WpfRectangle
            {
                Width = layout.Size,
                Height = layout.Size,
                Fill = new LinearGradientBrush(
                    MediaColor.FromRgb(9, 21, 31),
                    MediaColor.FromRgb(3, 8, 13),
                    35),
                IsHitTestVisible = false
            };
            Canvas.SetLeft(fallback, layout.Left);
            Canvas.SetTop(fallback, layout.Top);
            NpcMapCanvas.Children.Add(fallback);
        }

        var shade = new WpfRectangle
        {
            Width = layout.Size,
            Height = layout.Size,
            Fill = new LinearGradientBrush
            {
                StartPoint = new WpfPoint(0, 0),
                EndPoint = new WpfPoint(1, 1),
                GradientStops =
                {
                    new GradientStop(MediaColor.FromArgb(image == null ? (byte)20 : (byte)70, 3, 9, 14), 0),
                    new GradientStop(MediaColor.FromArgb(image == null ? (byte)90 : (byte)115, 3, 10, 16), 1)
                }
            },
            IsHitTestVisible = false
        };
        Canvas.SetLeft(shade, layout.Left);
        Canvas.SetTop(shade, layout.Top);
        NpcMapCanvas.Children.Add(shade);

        var border = new WpfRectangle
        {
            Width = Math.Max(0, layout.Size - 1),
            Height = Math.Max(0, layout.Size - 1),
            Stroke = new SolidColorBrush(MediaColor.FromArgb(180, 45, 78, 99)),
            StrokeThickness = 1,
            Fill = MediaBrushes.Transparent,
            IsHitTestVisible = false
        };
        Canvas.SetLeft(border, layout.Left + 0.5);
        Canvas.SetTop(border, layout.Top + 0.5);
        NpcMapCanvas.Children.Add(border);

        AddCanvasLabel(status, layout.Left + Math.Max(12, layout.Size - 300), layout.Top + layout.Size - 26, image == null ? "#F0B35A" : "#8CB7D6", 280, FontWeights.Normal);
    }

    private void DrawMinimapGrid(MapCanvasLayout layout, bool hasImage)
    {
        for (var i = 1; i < 4; i++)
        {
            var x = layout.Left + layout.Size * i / 4.0;
            var y = layout.Top + layout.Size * i / 4.0;
            var color = hasImage
                ? MediaColor.FromArgb(95, 132, 181, 210)
                : MediaColor.FromArgb(145, 25, 45, 60);
            NpcMapCanvas.Children.Add(new Line
            {
                X1 = x,
                Y1 = layout.Top,
                X2 = x,
                Y2 = layout.Top + layout.Size,
                Stroke = new SolidColorBrush(color),
                StrokeThickness = 1
            });
            NpcMapCanvas.Children.Add(new Line
            {
                X1 = layout.Left,
                Y1 = y,
                X2 = layout.Left + layout.Size,
                Y2 = y,
                Stroke = new SolidColorBrush(color),
                StrokeThickness = 1
            });
        }
    }

    private void DrawNpcMarker(NpcEditorRow npc, NpcEditorRow selected, MapCanvasLayout layout, int mapId, int mapWidth, int mapHeight)
    {
        var point = MapToCanvas(npc.MapPositionX, npc.MapPositionY, layout, mapId, mapWidth, mapHeight);
        var isSelected = ReferenceEquals(npc, selected) || npc.NpcId == selected.NpcId;
        var size = isSelected ? 16.0 : 9.0;
        var marker = new Ellipse
        {
            Width = size,
            Height = size,
            Fill = new SolidColorBrush(isSelected ? MediaColor.FromRgb(53, 208, 163) : MediaColor.FromRgb(104, 150, 190)),
            Stroke = new SolidColorBrush(isSelected ? MediaColor.FromRgb(124, 241, 210) : MediaColor.FromRgb(20, 34, 46)),
            StrokeThickness = isSelected ? 2 : 1,
            ToolTip = $"{npc.DisplayName}\nX {npc.MapPositionX}, Y {npc.MapPositionY}, Rot {npc.MapRotation:0.##}"
        };

        Canvas.SetLeft(marker, point.X - size / 2);
        Canvas.SetTop(marker, point.Y - size / 2);
        NpcMapCanvas.Children.Add(marker);

        if (isSelected)
            AddCanvasLabel(npc.NameTitle, Math.Min(layout.Left + layout.Size - 160, point.X + 10), Math.Max(layout.Top + 28, point.Y - 12), "#35D0A3");
    }

    private void AddCanvasLabel(string text, double left, double top, string color, double maxWidth = 320, FontWeight? fontWeight = null)
    {
        var label = new TextBlock
        {
            Text = text,
            Foreground = new SolidColorBrush((MediaColor)MediaColorConverter.ConvertFromString(color)),
            FontWeight = fontWeight ?? FontWeights.SemiBold,
            TextWrapping = TextWrapping.Wrap,
            MaxWidth = maxWidth,
            IsHitTestVisible = false
        };
        Canvas.SetLeft(label, left);
        Canvas.SetTop(label, top);
        NpcMapCanvas.Children.Add(label);
    }

    private static MapCanvasLayout GetMapCanvasLayout(double width, double height)
    {
        var side = Math.Max(1, Math.Min(Math.Max(1, width), Math.Max(1, height)));
        return new MapCanvasLayout(
            Math.Max(0, (width - side) / 2.0),
            Math.Max(0, (height - side) / 2.0),
            side);
    }

    private static bool IsInsideMapLayout(double canvasX, double canvasY, MapCanvasLayout layout)
        => canvasX >= layout.Left &&
           canvasX <= layout.Left + layout.Size &&
           canvasY >= layout.Top &&
           canvasY <= layout.Top + layout.Size;

    private static (double X, double Y) MapToCanvas(double mapX, double mapY, MapCanvasLayout layout, int mapId, int mapWidth, int mapHeight)
    {
        var (localX, localY) = MapToZoneLocal(mapX, mapY, layout.Size, mapId, mapWidth, mapHeight);
        return (layout.Left + Clamp(localX, 0, layout.Size), layout.Top + Clamp(localY, 0, layout.Size));
    }

    private static (int X, int Y) CanvasToMap(double canvasX, double canvasY, MapCanvasLayout layout, int mapId, int mapWidth, int mapHeight)
    {
        var localX = Clamp(canvasX - layout.Left, 0, layout.Size);
        var localY = Clamp(canvasY - layout.Top, 0, layout.Size);
        var (x, y) = ZoneLocalToMap(localX, localY, layout.Size, mapId, mapWidth, mapHeight);
        return ((int)Math.Round(x), (int)Math.Round(y));
    }

    private static (double X, double Y) MapToZoneLocal(double mapX, double mapY, double renderSize, int mapId, int mapWidth, int mapHeight)
    {
        var heightHigh = mapHeight >= mapWidth;
        var maxSize = Math.Max(1, Math.Max(mapWidth, mapHeight));
        var halfDelta = Math.Abs(mapHeight - mapWidth) * 0.5;
        var meterToPixel = renderSize / (mapId == 105 ? 55919.0 : maxSize);

        if (heightHigh)
        {
            return mapId == 105
                ? ((mapX - 1264) * meterToPixel, (maxSize - 22048 - mapY) * meterToPixel)
                : ((mapX + halfDelta) * meterToPixel, (maxSize - mapY) * meterToPixel);
        }

        return (mapX * meterToPixel, (maxSize - mapY - halfDelta) * meterToPixel);
    }

    private static (double X, double Y) ZoneLocalToMap(double localX, double localY, double renderSize, int mapId, int mapWidth, int mapHeight)
    {
        var heightHigh = mapHeight >= mapWidth;
        var maxSize = Math.Max(1, Math.Max(mapWidth, mapHeight));
        var halfDelta = Math.Abs(mapHeight - mapWidth) * 0.5;
        var meterToPixel = renderSize / (mapId == 105 ? 55919.0 : maxSize);

        if (heightHigh)
        {
            return mapId == 105
                ? (localX / meterToPixel + 1264, maxSize - 22048 - localY / meterToPixel)
                : (localX / meterToPixel - halfDelta, maxSize - localY / meterToPixel);
        }

        return (localX / meterToPixel, maxSize - halfDelta - localY / meterToPixel);
    }

    private static double Clamp(double value, double min, double max)
        => value < min ? min : value > max ? max : value;

    private void CraftListBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (CraftListBox.SelectedItem is NpcEditorRow npc)
            StatusText.Text = $"{npc.DisplayName} | craft entries {npc.Items.Count}";
    }

    private void ShopListBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (ShopListBox.SelectedItem is NpcEditorRow npc)
            StatusText.Text = $"{npc.DisplayName} | shop items {npc.Items.Count}";
    }

    private void QuestFilter_Changed(object sender, RoutedEventArgs e)
        => _questView?.Refresh();

    private void QuestListBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (QuestListBox.SelectedItem is QuestEditorRow quest)
            StatusText.Text = $"{quest.DisplayName} | {quest.Summary}";
    }

    private void ItemListBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (ItemListBox.SelectedItem is ItemEditorRow item)
        {
            _workspace.Reclassify(item);
            BindLinkedRows(item);
            StatusText.Text = $"{item.DisplayName} | {item.EffectKind}";
        }
        else
        {
            BindLinkedRows(null);
        }
    }

    private void BindLinkedRows(ItemEditorRow? item)
    {
        SkillPanel.DataContext = _workspace.ResolveSkill(item);
        BuffPanel.DataContext = _workspace.ResolveBuff(item);
        var container = _workspace.ResolveContainer(item);
        ContainerPanel.DataContext = container;
        RewardGrid.ItemsSource = container?.Rewards;
        if (container?.Rewards.Count > 0 && RewardGrid.SelectedItem == null)
            RewardGrid.SelectedIndex = 0;
        UpdateKindUi(item);
        RefreshRewardItemPicker();
        RefreshSkillPicker();
    }

    private void UpdateKindUi(ItemEditorRow? item)
    {
        if (item == null)
        {
            SkillSection.Visibility = Visibility.Collapsed;
            BuffSection.Visibility = Visibility.Collapsed;
            ContainerSection.Visibility = Visibility.Collapsed;
            ItemKindDetailText.Text = string.Empty;
            return;
        }

        var kind = _workspace.ResolveKind(item);
        _syncingKind = true;
        ItemKindComboBox.SelectedItem = ItemKindOptions.FirstOrDefault(x => x.Key == kind.Key);
        _syncingKind = false;
        ItemKindDetailText.Text = kind.Detail;

        var hasSkill = _workspace.ResolveSkill(item) != null;
        var hasBuff = _workspace.ResolveBuff(item) != null;
        var hasContainer = _workspace.ResolveContainer(item) != null;
        var isBox = kind.Key == ItemKindCatalog.Box;
        var isMegaphone = kind.Key == ItemKindCatalog.Megaphone;
        var isEquipment = kind.Key == ItemKindCatalog.Equipment || kind.Key == ItemKindCatalog.Accessory;
        var isStatusItem = kind.Key == ItemKindCatalog.UsableBuff ||
                           kind.Key == ItemKindCatalog.MemorySkill ||
                           kind.Key == ItemKindCatalog.Recovery ||
                           kind.Key == ItemKindCatalog.AttributeSocket;

        SkillSection.Visibility = !isBox && !isMegaphone && hasSkill && (isEquipment || isStatusItem)
            ? Visibility.Visible
            : Visibility.Collapsed;
        BuffSection.Visibility = !isBox && !isMegaphone && hasBuff && kind.Key != ItemKindCatalog.AttributeSocket
            ? Visibility.Visible
            : Visibility.Collapsed;
        ContainerSection.Visibility = isBox || hasContainer
            ? Visibility.Visible
            : Visibility.Collapsed;
    }

    private void ItemKindComboBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (_syncingKind || ItemListBox.SelectedItem is not ItemEditorRow item || ItemKindComboBox.SelectedItem is not ItemKindOption kind)
            return;

        item.TypeL = kind.DefaultTypeL;
        item.TypeS = kind.DefaultTypeS;
        if (kind.ClearsSkill)
            item.SkillCode = 0;

        if (kind.Key == ItemKindCatalog.Equipment || kind.Key == ItemKindCatalog.Accessory)
        {
            item.Overlap = 1;
            item.UseMode = 1;
        }
        else if (kind.Key == ItemKindCatalog.Box)
        {
            item.Overlap = Math.Max(item.Overlap, (short)1);
            item.UseMode = 0;
            _workspace.EnsureContainer(item);
        }
        else if (kind.Key == ItemKindCatalog.Megaphone)
        {
            item.Overlap = Math.Max(item.Overlap, (short)1);
            item.UseMode = 0;
        }

        _workspace.Reclassify(item);
        BindLinkedRows(item);
        _view?.Refresh();
        StatusText.Text = $"Item type changed to {kind.Name}.";
    }

    private void DurationPreset_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item || sender is not FrameworkElement { Tag: string tag } || !int.TryParse(tag, out var minutes))
            return;

        item.UsageTimeMinutes = minutes;
        item.UseTimeType = minutes <= 0 ? 0 : item.UseTimeType is 1 or 2 or 3 ? item.UseTimeType : 1;

        _workspace.Reclassify(item);
        _view?.Refresh();
        StatusText.Text = minutes <= 0
            ? "Item duration set to permanent."
            : $"Item duration set to {minutes} minutes.";
    }

    private void NewItem_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            var newItem = _workspace.CreateNewItem();
            _view?.Refresh();
            ItemListBox.SelectedItem = newItem;
            ItemListBox.ScrollIntoView(newItem);
            StatusText.Text = $"Created blank item {newItem.DisplayName}. Set name, icon, type, effect, and save ItemList.bin.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "New item failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void NewNpc_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            var selectedType = (NpcTypeFilterComboBox.SelectedItem as LookupOption)?.Id ?? 0;
            var selectedMap = (NpcMapFilterComboBox.SelectedItem as LookupOption)?.Id ?? 0;
            var npc = _assets.CreateNewNpc(Math.Max(selectedType, 0), "New NPC", Math.Max(selectedMap, 0));
            RefreshNpcMapFilterOptions();
            RefreshAssetViews();
            NpcListBox.SelectedItem = npc;
            NpcListBox.ScrollIntoView(npc);
            StatusText.Text = $"Created draft NPC {npc.DisplayName}. NPC bin saving is not wired yet.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "New NPC failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void NewQuest_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            var selectedType = (QuestTypeFilterComboBox.SelectedItem as LookupOption)?.Id ?? 0;
            var quest = _assets.CreateNewQuest(Math.Max(selectedType, 0));
            _questView?.Refresh();
            QuestListBox.SelectedItem = quest;
            QuestListBox.ScrollIntoView(quest);
            StatusText.Text = $"Created draft quest {quest.DisplayName}. Use Save All to write Quest.bin.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "New quest failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void NewCraft_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            CraftSearchBox.Text = string.Empty;
            var selectedMap = (CraftMapFilterComboBox.SelectedItem as LookupOption)?.Id ?? 0;
            var npc = _assets.CreateNewNpc(20, "New Craft NPC", Math.Max(selectedMap, 0));
            RefreshNpcMapFilterOptions();
            RefreshAssetViews();
            CraftListBox.SelectedItem = npc;
            CraftListBox.ScrollIntoView(npc);
            StatusText.Text = $"Created draft craft NPC {npc.DisplayName}. Add craft item rows before wiring bin save.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "New craft NPC failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void NewShop_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            ShopSearchBox.Text = string.Empty;
            var selectedType = (ShopTypeFilterComboBox.SelectedItem as LookupOption)?.Id ?? 1;
            var selectedMap = (ShopMapFilterComboBox.SelectedItem as LookupOption)?.Id ?? 0;
            var npc = _assets.CreateNewNpc(
                selectedType < 0 ? 1 : selectedType,
                "New Shop NPC",
                selectedMap < 0 ? 0 : selectedMap);
            RefreshNpcMapFilterOptions();
            RefreshAssetViews();
            ShopListBox.SelectedItem = npc;
            ShopListBox.ScrollIntoView(npc);
            StatusText.Text = $"Created draft shop NPC {npc.DisplayName}. Add shop item rows before wiring bin save.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "New shop NPC failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void RefreshAssetViews()
    {
        _npcView?.Refresh();
        _craftView?.Refresh();
        _shopView?.Refresh();
    }

    private void AddCraftRecipe_Click(object sender, RoutedEventArgs e)
    {
        if (CraftListBox.SelectedItem is not NpcEditorRow npc)
        {
            StatusText.Text = "Select a craft NPC first.";
            return;
        }

        var resultItem = ItemListBox.SelectedItem as ItemEditorRow;
        var recipe = _workspace.AddCraftRecipe(npc.NpcId, resultItem);
        if (!npc.CraftRecipes.Contains(recipe))
            npc.CraftRecipes.Add(recipe);

        npc.RefreshAssetSummary();
        RefreshAssetViews();
        CraftListBox.SelectedItem = npc;
        CraftRecipeGrid.SelectedItem = recipe;
        CraftRecipeGrid.ScrollIntoView(recipe);
        StatusText.Text = resultItem == null
            ? $"Created blank recipe {recipe.SequencialId} for {npc.DisplayName}."
            : $"Created recipe {recipe.SequencialId} for {resultItem.DisplayName}.";
    }

    private void AddCraftMaterial_Click(object sender, RoutedEventArgs e)
    {
        if (CraftRecipeGrid.SelectedItem is not CraftRecipeEditorRow recipe)
        {
            StatusText.Text = "Select a craft recipe first.";
            return;
        }

        var materialItem = ItemListBox.SelectedItem as ItemEditorRow;
        var material = _workspace.AddCraftMaterial(recipe, materialItem);
        CraftMaterialGrid.SelectedItem = material;
        CraftMaterialGrid.ScrollIntoView(material);
        StatusText.Text = materialItem == null
            ? $"Created blank material row for recipe {recipe.SequencialId}."
            : $"Added material {materialItem.DisplayName} to recipe {recipe.SequencialId}.";
    }

    private void RefreshCraftRecipe_Click(object sender, RoutedEventArgs e)
    {
        if (CraftListBox.SelectedItem is not NpcEditorRow npc)
            return;

        foreach (var recipe in npc.CraftRecipes)
            _workspace.ResolveCraftRecipeDisplay(recipe);

        npc.RefreshAssetSummary();
        CraftRecipeGrid.Items.Refresh();
        CraftMaterialGrid.Items.Refresh();
        StatusText.Text = $"Refreshed {npc.CraftRecipes.Count:N0} craft recipes for {npc.DisplayName}.";
    }

    private void CraftRecipeGrid_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (CraftRecipeGrid.SelectedItem is CraftRecipeEditorRow recipe)
            StatusText.Text = $"{recipe.DisplayName} | {recipe.Summary}";
    }

    private void AddQuestSupply_Click(object sender, RoutedEventArgs e)
    {
        if (QuestListBox.SelectedItem is not QuestEditorRow quest)
        {
            StatusText.Text = "Select a quest first.";
            return;
        }

        var supply = _assets.AddQuestSupply(quest, ItemListBox.SelectedItem as ItemEditorRow);
        QuestSupplyGrid.SelectedItem = supply;
        QuestSupplyGrid.ScrollIntoView(supply);
        StatusText.Text = $"Added supply to quest {quest.DisplayName}.";
    }

    private void AddQuestCondition_Click(object sender, RoutedEventArgs e)
    {
        if (QuestListBox.SelectedItem is not QuestEditorRow quest)
        {
            StatusText.Text = "Select a quest first.";
            return;
        }

        var condition = _assets.AddQuestCondition(quest);
        QuestConditionGrid.SelectedItem = condition;
        QuestConditionGrid.ScrollIntoView(condition);
        StatusText.Text = $"Added precondition to quest {quest.DisplayName}.";
    }

    private void AddQuestGoal_Click(object sender, RoutedEventArgs e)
    {
        if (QuestListBox.SelectedItem is not QuestEditorRow quest)
        {
            StatusText.Text = "Select a quest first.";
            return;
        }

        var goal = _assets.AddQuestGoal(quest, ItemListBox.SelectedItem as ItemEditorRow);
        QuestGoalGrid.SelectedItem = goal;
        QuestGoalGrid.ScrollIntoView(goal);
        StatusText.Text = $"Added completion requirement to quest {quest.DisplayName}.";
    }

    private void QuestGoalCombo_DropDownClosed(object sender, EventArgs e)
    {
        RefreshQuestGoalContext();
    }

    private void QuestGoalField_LostFocus(object sender, RoutedEventArgs e)
    {
        RefreshQuestGoalContext();
    }

    private void RefreshQuestGoalContext()
    {
        if (!IsLoaded || _refreshingQuestGoalContext)
            return;

        if (QuestListBox.SelectedItem is not QuestEditorRow quest)
            return;

        try
        {
            _refreshingQuestGoalContext = true;
            _assets.RefreshQuestDerivedData(quest);
        }
        finally
        {
            _refreshingQuestGoalContext = false;
        }
    }

    private void AddQuestReward_Click(object sender, RoutedEventArgs e)
    {
        if (QuestListBox.SelectedItem is not QuestEditorRow quest)
        {
            StatusText.Text = "Select a quest first.";
            return;
        }

        var reward = _assets.AddQuestReward(quest, ItemListBox.SelectedItem as ItemEditorRow);
        QuestRewardGrid.SelectedItem = reward;
        QuestRewardGrid.ScrollIntoView(reward);
        StatusText.Text = $"Added reward to quest {quest.DisplayName}.";
    }

    private void AddQuestEvent_Click(object sender, RoutedEventArgs e)
    {
        if (QuestListBox.SelectedItem is not QuestEditorRow quest)
        {
            StatusText.Text = "Select a quest first.";
            return;
        }

        var evt = _assets.AddQuestEvent(quest);
        QuestEventGrid.SelectedItem = evt;
        QuestEventGrid.ScrollIntoView(evt);
        StatusText.Text = $"Added event slot to quest {quest.DisplayName}.";
    }

    private void Duplicate_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            var newItem = _workspace.Duplicate(ItemListBox.SelectedItem as ItemEditorRow);
            _view?.Refresh();
            ItemListBox.SelectedItem = newItem;
            StatusText.Text = $"Created {newItem.DisplayName}.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "Duplicate failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private async void Save_Click(object sender, RoutedEventArgs e)
    {
        var dirtyDbContainers = _workspace.DirtyContainers();
        var dbSyncMessage = dirtyDbContainers.Count > 0
            ? $" It will also sync {dirtyDbContainers.Count:N0} changed box container(s) to the configured database."
            : string.Empty;

        var result = System.Windows.MessageBox.Show(
            "This will save every changed asset bin back to the loaded folder/pack: ItemList.bin, linked Skill/Buff/Container bins, Npc.bin, NpcAssets.bin, MapNpc.bin, and Quest.bin. Timestamped backups are created first." + dbSyncMessage + " Continue?",
            "Save all changed assets",
            MessageBoxButton.YesNo,
            MessageBoxImage.Warning);

        if (result != MessageBoxResult.Yes)
            return;

        try
        {
            var messages = new List<string> { _workspace.Save() };
            messages.AddRange(_assets.SaveAllDirty());
            if (dirtyDbContainers.Count > 0)
            {
                var connectionString = DbConnectionBox.Text.Trim();
                if (string.IsNullOrWhiteSpace(connectionString))
                {
                    messages.Add("Database box sync skipped because the DB connection string is empty.");
                    SetDbStatus("Database box sync skipped: empty connection string.", "Warn");
                }
                else
                {
                    SetDbControlsEnabled(false);
                    _database.SaveConnectionString(connectionString);
                    SetDbStatus($"Saving {dirtyDbContainers.Count:N0} changed box container(s) to database...", "Warn");
                    foreach (var container in dirtyDbContainers)
                        await _database.SaveContainerAsync(connectionString, container);

                    var dbMessage = $"Synced {dirtyDbContainers.Count:N0} changed box container(s) to database.";
                    messages.Add(dbMessage);
                    SetDbStatus(dbMessage, "Accent");
                }
            }

            var message = string.Join(" ", messages.Where(x => !string.IsNullOrWhiteSpace(x)));
            StatusText.Text = message;
            _view?.Refresh();
            _npcView?.Refresh();
            _craftView?.Refresh();
            _shopView?.Refresh();
            _questView?.Refresh();
            RenderNpcMinimap();
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.ToString(), "Save failed", MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText.Text = "Save failed.";
        }
        finally
        {
            SetDbControlsEnabled(true);
        }
    }

    private async void TestDb_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            SetDbControlsEnabled(false);
            var connectionString = DbConnectionBox.Text.Trim();
            _database.SaveConnectionString(connectionString);
            SetDbStatus("Testing database connection...", "Warn");
            StatusText.Text = "Testing database connection...";
            var message = await _database.TestConnectionAsync(connectionString);
            SetDbStatus(message, "Accent");
            StatusText.Text = message;
        }
        catch (Exception ex)
        {
            SetDbStatus($"Database connection failed: {ex.Message}", "Danger");
            System.Windows.MessageBox.Show(ex.Message, "Database connection failed", MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText.Text = "Database connection failed.";
        }
        finally
        {
            SetDbControlsEnabled(true);
        }
    }

    private async void LoadDbBoxes_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            SetDbControlsEnabled(false);
            var connectionString = DbConnectionBox.Text.Trim();
            _database.SaveConnectionString(connectionString);
            SetDbStatus("Loading box rewards from database...", "Warn");
            StatusText.Text = "Loading boxes from database...";
            var containers = await _database.LoadContainersAsync(connectionString);
            var count = _workspace.MergeContainersFromDatabase(containers);

            _view?.Refresh();
            if (ItemListBox.SelectedItem is ItemEditorRow item)
                BindLinkedRows(item);

            var message = $"Loaded {count:N0} box container rows from database.";
            SetDbStatus(message, "Accent");
            StatusText.Text = message;
        }
        catch (Exception ex)
        {
            SetDbStatus($"Database load failed: {ex.Message}", "Danger");
            System.Windows.MessageBox.Show(ex.ToString(), "Database load failed", MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText.Text = "Database load failed.";
        }
        finally
        {
            SetDbControlsEnabled(true);
        }
    }

    private async void SaveBoxDb_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
        {
            SetDbStatus("Select a box item before saving to database.", "Warn");
            StatusText.Text = "Select a box item first.";
            return;
        }

        var container = _workspace.ResolveContainer(item) ?? _workspace.EnsureContainer(item);
        var result = System.Windows.MessageBox.Show(
            $"This will update production DB container rewards for item {item.ItemId} - {item.NameTitle}. Continue?",
            "Save selected box to database",
            MessageBoxButton.YesNo,
            MessageBoxImage.Warning);

        if (result != MessageBoxResult.Yes)
            return;

        try
        {
            SetDbControlsEnabled(false);
            var connectionString = DbConnectionBox.Text.Trim();
            _database.SaveConnectionString(connectionString);
            SetDbStatus($"Saving box {item.ItemId} to database...", "Warn");
            StatusText.Text = $"Saving box {item.ItemId} to database...";
            await _database.SaveContainerAsync(connectionString, container);
            _workspace.Reclassify(item);
            BindLinkedRows(item);
            _view?.Refresh();
            var message = $"Saved box {item.ItemId} rewards directly to database.";
            SetDbStatus(message, "Accent");
            StatusText.Text = message;
        }
        catch (Exception ex)
        {
            SetDbStatus($"Database save failed: {ex.Message}", "Danger");
            System.Windows.MessageBox.Show(ex.ToString(), "Database save failed", MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText.Text = "Database save failed.";
        }
        finally
        {
            SetDbControlsEnabled(true);
        }
    }

    private void Export_Click(object sender, RoutedEventArgs e)
    {
        using var dialog = new Forms.SaveFileDialog
        {
            Title = "Export item analysis",
            Filter = "JSON|*.json",
            FileName = "lkdmo-item-analysis.json",
            InitialDirectory = Directory.Exists(_workspace.BinDirectory) ? _workspace.BinDirectory : @"C:\0-NewDmo"
        };

        if (dialog.ShowDialog() != Forms.DialogResult.OK)
            return;

        _workspace.ExportJson(dialog.FileName);
        StatusText.Text = $"Exported {dialog.FileName}.";
    }

    private void SetBuff_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
            return;

        item.TypeL = 63;
        _workspace.Reclassify(item);
        BindLinkedRows(item);
        StatusText.Text = "Item marked as buff consumable. Set Skill Code to the desired skill/buff link.";
    }

    private void SetBox_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
            return;

        item.TypeL = 170;
        _workspace.EnsureContainer(item);
        _workspace.Reclassify(item);
        BindLinkedRows(item);
        StatusText.Text = "Item marked as box. Add reward rows and configure Chance %, Min, Max, and Rare.";
    }

    private void SetRecovery_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
            return;

        item.TypeL = 61;
        _workspace.Reclassify(item);
        BindLinkedRows(item);
        StatusText.Text = "Item marked as recovery consumable. Set Skill Code and Type S target.";
    }

    private void RefreshEffect_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
            return;

        _workspace.Reclassify(item);
        BindLinkedRows(item);
        StatusText.Text = item.EffectSummary;
    }

    private void SkillSearchBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        => RefreshSkillPicker();

    private void SkillPickerList_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        => ApplySelectedSkill();

    private void ApplySkill_Click(object sender, RoutedEventArgs e)
        => ApplySelectedSkill();

    private void CloneSkill_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
            return;

        try
        {
            var skill = _workspace.DuplicateSkillForItem(item);
            RefreshSkillPicker();
            BindLinkedRows(item);
            StatusText.Text = $"Created linked skill {skill.Title} for {item.DisplayName}.";
        }
        catch (Exception ex)
        {
            System.Windows.MessageBox.Show(ex.Message, "Clone skill failed", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void RefreshSkillPicker()
    {
        if (SkillPickerList == null)
            return;

        SkillPickerList.ItemsSource = _workspace.SearchSkills(SkillSearchBox.Text);
    }

    private void ApplySelectedSkill()
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow item)
        {
            StatusText.Text = "Select an item first.";
            return;
        }

        if (SkillPickerList.SelectedItem is not SkillEditorRow skill)
        {
            StatusText.Text = "Select a Skill.bin row first.";
            return;
        }

        item.SkillCode = skill.Id;
        _workspace.Reclassify(item);
        BindLinkedRows(item);
        StatusText.Text = $"SkillCode changed to {skill.Title}.";
    }

    private void RewardItemSearchBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        => RefreshRewardItemPicker();

    private void RewardGrid_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
    {
        if (RewardGrid.SelectedItem is ContainerRewardEditorRow reward)
            StatusText.Text = $"Selected reward {reward.DisplayName}.";
    }

    private void AddReward_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow boxItem)
        {
            StatusText.Text = "Select a box item first.";
            return;
        }

        var selectedRewardItem = RewardItemPickerList.SelectedItem as ItemEditorRow;
        var reward = _workspace.AddContainerReward(boxItem, selectedRewardItem);
        _workspace.Reclassify(boxItem);
        BindLinkedRows(boxItem);
        RewardGrid.SelectedItem = reward;
        RewardGrid.ScrollIntoView(reward);
        StatusText.Text = selectedRewardItem == null
            ? "Created empty reward row. Choose an item, then set Chance %, Min, Max, and Rare."
            : $"Added reward {selectedRewardItem.DisplayName} with 100% chance.";
    }

    private void RemoveReward_Click(object sender, RoutedEventArgs e)
    {
        if (ItemListBox.SelectedItem is not ItemEditorRow boxItem || RewardGrid.SelectedItem is not ContainerRewardEditorRow reward)
        {
            StatusText.Text = "Select a reward row to remove.";
            return;
        }

        _workspace.RemoveContainerReward(boxItem, reward);
        _workspace.Reclassify(boxItem);
        BindLinkedRows(boxItem);
        StatusText.Text = $"Removed reward {reward.DisplayName}.";
    }

    private void RewardItemPickerList_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        => ApplySelectedRewardItem();

    private void ApplyRewardItem_Click(object sender, RoutedEventArgs e)
        => ApplySelectedRewardItem();

    private void RefreshRewardItemPicker()
    {
        if (RewardItemPickerList == null)
            return;

        RewardItemPickerList.ItemsSource = _workspace.SearchRewardItems(RewardItemSearchBox.Text);
    }

    private void ApplySelectedRewardItem()
    {
        if (RewardItemPickerList.SelectedItem is not ItemEditorRow item)
        {
            StatusText.Text = "Select an item from ItemList.bin first.";
            return;
        }

        if (ItemListBox.SelectedItem is not ItemEditorRow boxItem)
        {
            StatusText.Text = "Select a box item first.";
            return;
        }

        var reward = RewardGrid.SelectedItem as ContainerRewardEditorRow
            ?? _workspace.AddContainerReward(boxItem, item);

        reward.ItemId = item.ItemId;
        reward.ItemName = item.NameTitle;
        _workspace.Reclassify(boxItem);
        BindLinkedRows(boxItem);
        RewardGrid.SelectedItem = reward;
        StatusText.Text = $"Reward updated to {item.DisplayName}.";
    }
}
