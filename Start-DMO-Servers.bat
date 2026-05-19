@echo off
setlocal

set "ROOT=%~dp0"
set "SERVER_ROOT=%ROOT%dmo-server-main"
set "CONFIGURATION=Debug"
set "FRAMEWORK=net7.0"

set "DB_CONNECTION=Server=104.234.41.94,1433;Database=ODMO;User Id=sa;Password=Essasenha2020@@@;TrustServerCertificate=True;"

set "ASPNETCORE_ENVIRONMENT=Development"
set "DOTNET_ENVIRONMENT=Development"
set "DSO_Database__Connection=%DB_CONNECTION%"
set "DSO_DB_CONNECTION=%DB_CONNECTION%"
set "DSO_GameServer__Port=7608"

if not exist "%SERVER_ROOT%\DigitalWorldOnline.sln" (
    echo Nao encontrei o projeto do servidor em:
    echo "%SERVER_ROOT%"
    pause
    exit /b 1
)

cd /d "%SERVER_ROOT%"

echo.
echo Encerrando servidores antigos, se existirem...
taskkill /F /IM dotnet.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Account.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Character.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Game.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Routine.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Admin.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Api.exe > nul 2>&1
timeout /t 2 /nobreak > nul 2>&1

echo.
echo Compilando servidor (%CONFIGURATION%)...
dotnet build "%SERVER_ROOT%\DigitalWorldOnline.sln" -c %CONFIGURATION% --nologo --verbosity:minimal
if errorlevel 1 (
    echo.
    echo A compilacao falhou. Corrija os erros acima antes de iniciar o servidor.
    pause
    exit /b 1
)

echo.
echo Iniciando DMO Server com o banco configurado em DSO_Database__Connection...
echo.

echo [1/6] Account.Host
start "DSO - Account" /D "%SERVER_ROOT%\src\Source\Distribution\DigitalWorldOnline.Account.Host\bin\%CONFIGURATION%\%FRAMEWORK%" cmd /k "DigitalWorldOnline.Account.exe"

echo [2/6] Character.Host
start "DSO - Character" /D "%SERVER_ROOT%\src\Source\Distribution\DigitalWorldOnline.Character.Host\bin\%CONFIGURATION%\%FRAMEWORK%" cmd /k "DigitalWorldOnline.Character.exe"

echo [3/6] Game.Host
start "DSO - Game" /D "%SERVER_ROOT%\src\Source\Distribution\DigitalWorldOnline.Game.Host\bin\%CONFIGURATION%\%FRAMEWORK%" cmd /k "DigitalWorldOnline.Game.exe"

echo [4/6] Routine.Host
start "DSO - Routine" /D "%SERVER_ROOT%\src\Source\Distribution\DigitalWorldOnline.Routine.Host\DigitalWorldOnline.Routine\bin\%CONFIGURATION%\%FRAMEWORK%" cmd /k "DigitalWorldOnline.Routine.exe"

echo [5/6] Account.Api
start "DSO - Api" /D "%SERVER_ROOT%\src\Source\Distribution\DigitalWorldOnline.Account.Api\bin\%CONFIGURATION%\%FRAMEWORK%" cmd /k "set ASPNETCORE_URLS=http://127.0.0.1:5189&& DigitalWorldOnline.Api.exe"

echo [6/6] Admin
start "DSO - Admin" /D "%SERVER_ROOT%\src\Source\Distribution\DigitalWorldOnline.Admin\bin\%CONFIGURATION%\%FRAMEWORK%" cmd /k "set ASPNETCORE_URLS=http://127.0.0.1:5000&& DigitalWorldOnline.Admin.exe"

echo.
echo Servidores iniciados em janelas separadas.
pause
