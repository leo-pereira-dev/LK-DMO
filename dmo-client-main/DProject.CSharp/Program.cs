using DProject.App;
using DProject.Config;

namespace DProject;

internal static class Program
{
    private static int Main(string[] args)
    {
        AppConfig config = AppConfig.Load("appsettings.json");

        using var gameApp = new CGameApp(config, args);
        if (!gameApp.Startup(
                config.Window.Title,
                config.Window.Width,
                config.Window.Height,
                config.Window.Fullscreen))
        {
            return 1;
        }

        return gameApp.Run();
    }
}

