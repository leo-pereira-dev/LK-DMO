using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.GameAssets.Mapping;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Application.Admin.Repositories;
using DigitalWorldOnline.Application.Extensions;
using DigitalWorldOnline.Application.Routines.Commands;
using DigitalWorldOnline.Application.Routines.Queries;
using DigitalWorldOnline.Application.Services;
using DigitalWorldOnline.Commons.DTOs.Routine;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Repositories.Admin;
using DigitalWorldOnline.Infraestructure;
using DigitalWorldOnline.Infraestructure.Extensions;
using DigitalWorldOnline.Infraestructure.Mapping;
using DigitalWorldOnline.Infraestructure.Repositories.Account;
using DigitalWorldOnline.Infraestructure.Repositories.Admin;
using DigitalWorldOnline.Infraestructure.Repositories.Character;
using DigitalWorldOnline.Infraestructure.Repositories.Config;
using DigitalWorldOnline.Infraestructure.Repositories.Routine;
using DigitalWorldOnline.Infraestructure.Repositories.Server;
using MediatR;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Serilog;
using System.Globalization;
using System.Reflection;

namespace DigitalWorldOnline.Routine
{
    public class Program
    {
        public static void Main(string[] args)
        {
            try
            {
                CreateHostBuilder(args).Run();
            }
            catch (Exception ex)
            {
                Console.WriteLine("FATAL ERROR in Main:");
                Console.WriteLine($"Exception: {ex.GetType().Name}");
                Console.WriteLine($"Message: {ex.Message}");
                Console.WriteLine($"StackTrace: {ex.StackTrace}");
                if (ex.InnerException != null)
                {
                    Console.WriteLine($"Inner Exception: {ex.InnerException.GetType().Name}");
                    Console.WriteLine($"Inner Message: {ex.InnerException.Message}");
                    Console.WriteLine($"Inner StackTrace: {ex.InnerException.StackTrace}");
                }
                Console.ReadLine();
                throw;
            }
        }

        private static void UnhandledExceptionHandler(object sender, UnhandledExceptionEventArgs e)
        {
            if (e.IsTerminating)
            {
                Console.WriteLine("Terminating by unhandled exception...");
                Console.WriteLine($"Exception: {e.ExceptionObject}");
                if (e.ExceptionObject is Exception ex)
                {
                    Console.WriteLine($"Message: {ex.Message}");
                    Console.WriteLine($"StackTrace: {ex.StackTrace}");
                    if (ex.InnerException != null)
                    {
                        Console.WriteLine($"Inner Exception: {ex.InnerException.Message}");
                        Console.WriteLine($"Inner StackTrace: {ex.InnerException.StackTrace}");
                    }
                }
            }
            else
                Console.WriteLine("Received unhandled exception.");

            Console.ReadLine();
        }

        public static IHost CreateHostBuilder(string[] args)
        {
            AppDomain.CurrentDomain.UnhandledException += UnhandledExceptionHandler;
            Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;

            var host = Host.CreateDefaultBuilder(args)
                .UseSerilog()
                .UseEnvironment("Development")
                .ConfigureServices((context, services) =>
                {
                    services.AddDsoDatabase(context.Configuration);

                    //TODO: remover após segregar
                    services.AddScoped<IAdminQueriesRepository, AdminQueriesRepository>();
                    services.AddScoped<IAdminCommandsRepository, AdminCommandsRepository>();
                    services.AddScoped<IAccountQueriesRepository, AccountQueriesRepository>();
                    services.AddScoped<IAccountCommandsRepository, AccountCommandsRepository>();
                    services.AddScoped<IServerQueriesRepository, ServerQueriesRepository>();
                    services.AddScoped<IServerCommandsRepository, ServerCommandsRepository>();
                    services.AddScoped<ICharacterQueriesRepository, CharacterQueriesRepository>();
                    services.AddScoped<ICharacterCommandsRepository, CharacterCommandsRepository>();
                    services.AddScoped<IConfigQueriesRepository, ConfigQueriesRepository>();
                    services.AddScoped<IConfigCommandsRepository, ConfigCommandsRepository>();
                    services.AddAutoMapper(typeof(AccountProfile));
                    services.AddAutoMapper(typeof(GameAssetsProfile));
                    services.AddAutoMapper(typeof(CharacterProfile));
                    services.AddAutoMapper(typeof(ConfigProfile));
                    services.AddAutoMapper(typeof(DigimonProfile));
                    services.AddAutoMapper(typeof(GameProfile));
                    services.AddAutoMapper(typeof(SecurityProfile));
                    services.AddAutoMapper(typeof(ArenaProfile));
                    services.AddAutoMapper(typeof(RoutineProfile));
                    services.AddScoped<IRoutineRepository, RoutineRepository>();
                    services.AddSingleton<ISender, ScopedSender<Mediator>>();
                    services.AddSingleton(ConfigureLogger(context.Configuration));
                    services.AddHostedService<RoutineServer>();
                    services.AddTransient<Mediator>();
                    // MonsterBinLoader is required by AssetsLoader's constructor (catalog
                    // access for the bin-driven SUMMON_MONSTER path in Game.Host).  Routine
                    // doesn't consume the catalog but the DI graph still needs it satisfied.
                    services.AddSingleton<DMBaseBinLoader>();
                    services.AddSingleton<DigimonListBinLoader>();
                    services.AddSingleton<DigimonEvoBinLoader>();
                    services.AddSingleton<BuffBinLoader>();
                    services.AddSingleton<AchieveBinLoader>();
                    services.AddSingleton<EventTableBinLoader>();
                    services.AddSingleton<CashShopBinLoader>();
                    services.AddSingleton<SkillBinLoader>();
                    services.AddSingleton<NatureBinLoader>();
                    services.AddSingleton<MonsterBinLoader>();
                    services.AddSingleton<MapBinLoader>();
                    services.AddSingleton<ItemListBinLoader>();
                    services.AddSingleton<UnionXmlAssetLoader>();
                    services.AddSingleton<DUnitCollectionService>();
                    services.AddSingleton<AssetsLoader>();
                    services.AddScoped<StoragePersistenceDualWriteCoordinator>();
                    services.AddMediatR(
                        typeof(MediatorApplicationHandlerExtension).GetTypeInfo().Assembly,
                        typeof(GameAssetsMarker).GetTypeInfo().Assembly);
                    services.AddScoped<IRequestHandler<GetActiveRoutinesQuery, List<RoutineDTO>>, GetActiveRoutinesQueryHandler>();
                    services.AddScoped<IRequestHandler<UpdateRoutineExecutionTimeCommand>, UpdateRoutineExecutionTimeCommandHandler>();
                    services.AddScoped<IRequestHandler<ExecuteDailyQuestsRoutineCommand>, ExecuteDailyQuestsRoutineCommandHandler>();
                })
                .ConfigureAppConfiguration((context, config) =>
                {
                    var projectDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    config.SetBasePath(projectDir)
                          .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true)
                          .AddJsonFile($"appsettings.{context.HostingEnvironment.EnvironmentName}.json", optional: true, reloadOnChange: true)
                          .AddEnvironmentVariables("DSO_");
                })
                .Build();

            // Eagerly load static-data bins so a missing/corrupt file fails fast at boot
            var dmBase = host.Services.GetRequiredService<DMBaseBinLoader>().Load();
            var digimonList = host.Services.GetRequiredService<DigimonListBinLoader>().Load();
            var digimonEvo = host.Services.GetRequiredService<DigimonEvoBinLoader>().Load();
            var buff = host.Services.GetRequiredService<BuffBinLoader>().Load();
            var achieve = host.Services.GetRequiredService<AchieveBinLoader>().Load();
            var eventTable = host.Services.GetRequiredService<EventTableBinLoader>().Load();
            var cashShop = host.Services.GetRequiredService<CashShopBinLoader>().Load();
            var skill = host.Services.GetRequiredService<SkillBinLoader>().Load();
            var nature = host.Services.GetRequiredService<NatureBinLoader>().Load();
            var monster = host.Services.GetRequiredService<MonsterBinLoader>().Load();
            var map = host.Services.GetRequiredService<MapBinLoader>().Load();

            return host;
        }

        private static ILogger ConfigureLogger(IConfiguration configuration)
        {
            return new LoggerConfiguration()
                .ReadFrom.Configuration(configuration)
                .Enrich.FromLogContext()
                .WriteTo.Console(outputTemplate: "[{Timestamp:HH:mm:ss} {Level:u3}] {Message:lj}{NewLine}{Exception}")
                .WriteTo.RollingFile(configuration["Log:DebugRepository"] ?? "logs\\Debug\\RoutineServer", Serilog.Events.LogEventLevel.Information, retainedFileCountLimit: 10)
                .CreateLogger();
        }
    }
}
