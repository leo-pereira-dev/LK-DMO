@echo off
setlocal

set "SERVER_ROOT=%~dp0dmo-server-main-postgres"

if not exist "%SERVER_ROOT%\docker-compose.yml" (
    echo Nao encontrei o docker-compose.yml em:
    echo "%SERVER_ROOT%"
    pause
    exit /b 1
)

cd /d "%SERVER_ROOT%"

echo.
echo Subindo DMO Server PostgreSQL via Docker...
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
echo Servicos PostgreSQL iniciados.
echo Postgres: 127.0.0.1:5432 database=dso user=dso password=dso_password
echo Account: 127.0.0.1:17029
echo Character: 127.0.0.1:17050
echo Game: 127.0.0.1:17608
echo API: http://127.0.0.1:15189
echo Admin: http://127.0.0.1:15000
pause
