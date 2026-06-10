# Deploy do site LKDMO

## Ambientes

- `Development`: usa `wwwroot/appsettings.Development.json` e aponta para a API local Docker em `http://127.0.0.1:15189/`.
- `Production`: usa `wwwroot/appsettings.Production.json`, gerado no publish com `LKDMO_SITE_API_BASE_URL`.

## Variaveis da maquina

Configure uma vez no Windows:

```powershell
[Environment]::SetEnvironmentVariable("LKDMO_SITE_API_BASE_URL", "http://104.234.41.94:15189/", "User")
[Environment]::SetEnvironmentVariable("LKDMO_FTP_HOST", "212.85.9.65", "User")
[Environment]::SetEnvironmentVariable("LKDMO_FTP_USER", "u118259839.lkdmo.com.br", "User")
[Environment]::SetEnvironmentVariable("LKDMO_FTP_PORT", "21", "User")
[Environment]::SetEnvironmentVariable("LKDMO_FTP_REMOTE_PATH", "", "User")
[Environment]::SetEnvironmentVariable("LKDMO_FTP_PASSWORD", "sua-senha", "User")
```

`LKDMO_FTP_REMOTE_PATH` fica vazio porque esse usuario FTP ja entra direto em `/public_html`.

## Publicar localmente

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\Scripts\publish-site.ps1 -Environment Production
```

## Enviar para FTP

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\Scripts\deploy-ftp.ps1
```
