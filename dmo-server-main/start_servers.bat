@echo off
set ASPNETCORE_ENVIRONMENT=Development
set DSO_GameServer__Port=7608

REM Load local credentials (start_servers.local.bat is gitignored).
REM Create that file with your real DB password, e.g.:
REM   set DSO_Database__Connection=Server=localhost;Port=3306;Database=dso;Uid=root;Pwd=YOUR_PASSWORD;SslMode=None;AllowPublicKeyRetrieval=True;
if exist "%~dp0start_servers.local.bat" call "%~dp0start_servers.local.bat"

echo Checking for existing server processes...
taskkill /F /IM dotnet.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Account.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Character.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Game.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Routine.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Admin.exe > nul 2>&1
taskkill /F /IM DigitalWorldOnline.Api.exe > nul 2>&1
timeout /t 3 /nobreak > nul 2>&1
echo Killed existing server processes.
echo.

echo Starting DMO Server...
echo.

if not exist runlogs mkdir runlogs

echo [1/6] Starting Account.Host...
start "DSO - Account" /D "%CD%\src\Source\Distribution\DigitalWorldOnline.Account.Host\bin\Debug\net7.0" DigitalWorldOnline.Account.exe

echo.
echo [2/6] Starting Character.Host...
start "DSO - Character" /D "%CD%\src\Source\Distribution\DigitalWorldOnline.Character.Host\bin\Debug\net7.0" DigitalWorldOnline.Character.exe

echo.
echo [3/6] Starting Game.Host...
start "DSO - Game" /D "%CD%\src\Source\Distribution\DigitalWorldOnline.Game.Host\bin\Debug\net7.0" DigitalWorldOnline.Game.exe

echo.
echo [4/6] Starting Routine.Host...
start "DSO - Routine" /D "%CD%\src\Source\Distribution\DigitalWorldOnline.Routine.Host\DigitalWorldOnline.Routine\bin\Debug\net7.0" DigitalWorldOnline.Routine.exe

echo.
echo [5/6] Starting Account.Api...
start "DSO - Api" /D "%CD%\src\Source\Distribution\DigitalWorldOnline.Account.Api\bin\Debug\net7.0" cmd /c "set ASPNETCORE_URLS=http://127.0.0.1:5189&& DigitalWorldOnline.Api.exe"

echo.
echo [6/6] Starting Admin...
start "DSO - Admin" /D "%CD%\src\Source\Distribution\DigitalWorldOnline.Admin\bin\Debug\net7.0" cmd /c "set ASPNETCORE_URLS=http://127.0.0.1:5000&& DigitalWorldOnline.Admin.exe"

echo.
echo All servers started in separate windows.
pause
