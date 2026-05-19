@echo off
setlocal

set "SERVER_ROOT=%~dp0dmo-server-main"

if not exist "%SERVER_ROOT%\docker-compose.yml" (
    echo Nao encontrei o docker-compose.yml em:
    echo "%SERVER_ROOT%"
    pause
    exit /b 1
)

cd /d "%SERVER_ROOT%"

echo.
echo Subindo MySQL/MariaDB e servidores DMO via Docker...
docker compose up -d --build
if errorlevel 1 (
    echo.
    echo Docker compose falhou. Veja os erros acima.
    pause
    exit /b 1
)

echo.
docker compose ps
echo.
echo Servicos iniciados.
echo MySQL: 127.0.0.1:3306 database=dso user=dso password=dso_password
echo Account: 127.0.0.1:7029
echo Character: 127.0.0.1:7050
echo Game: 127.0.0.1:7608
echo API: http://127.0.0.1:5189
echo Admin: http://127.0.0.1:5000
pause
