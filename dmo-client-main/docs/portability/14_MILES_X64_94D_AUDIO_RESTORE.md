# Miles X64 9.4d Audio Restore

Data: 2026-06-07

## Objetivo

Restaurar audio no build `X64_WinDX9_Bridge|x64` sem manter o stub de Miles e sem alterar renderer, logica de jogo ou protocolo.

## Causa real

O problema nao era "mss64 e incompativel com mss64" de forma generica. O erro era mistura de conjuntos:

```text
mss64.dll 7.0x
providers mss64*.flt / mss64mp3.asi 9.4d
```

Depois o usuario adicionou:

```text
C:\dmo-client-main\mss64.dll
```

Validacao:

```text
ProductVersion: 9.4d
SHA256: CD9DBECE13DA468636308D8C424FF9C30B9A40F2772ABE298F6C09AE0FC01500
```

Esse arquivo e identico ao `redist64\mss64.dll` extraido do SDK local:

```text
C:\dmo-client-main\LibProj\MilesSoundX64_94d\redist64\mss64.dll
```

## Segundo problema encontrado

O executavel antigo importava:

```text
AIL_quick_startup
AIL_quick_shutdown
AIL_quick_handles
```

A `mss64.dll` 9.4d nao exporta essas funcoes. A `mss64.lib` 9.4d tambem nao possui esses simbolos.

Classificacao:

```text
erro de dependencia / API Miles antiga
```

## Correcoes aplicadas

### Gamebryo NiMilesAudio

Arquivo:

```text
D:\Gamebryo2.3\CoreLibs\NiAudio\Miles\NiMilesAudioSystem.cpp
```

Em `DMO_X64_WINDX9_BRIDGE`, a inicializacao deixou de usar `AIL_quick_startup` e passou a usar:

```cpp
AIL_startup();
m_pDIG = AIL_open_digital_driver(44100, 16, MSS_MC_USE_SYSTEM_CONFIG, 0);
```

No shutdown x64, `AIL_quick_shutdown()` nao e chamado.

### Client CSoundMgr

Arquivo:

```text
C:\dmo-client-main\DProject\CSoundMgr.cpp
```

O stub temporario x64 foi removido. `_InitMiles()` voltou a chamar:

```cpp
pkSS->Startup("Data\\NP\\Sound")
```

### Link do client

Arquivo:

```text
C:\dmo-client-main\DProject\DProject.vcxproj
```

O build x64 agora busca `mss64.lib` em:

```text
C:\dmo-client-main\LibProj\MilesSoundX64_94d\lib
```

## Build executado

Rebuild do Gamebryo audio:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "D:\Gamebryo2.3\CoreLibs\NiAudio\Miles\Win32\VC80\NiMilesAudio.vcxproj" `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\nimilesaudio_x64_94d_build.log;Verbosity=normal"
```

Resultado:

```text
Build OK
D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib\NiMilesAudio.lib
```

Rebuild do client:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_miles94d_enabled.log;Verbosity=normal"
```

Resultado:

```text
Build OK
C:\Release\GSP_x64\lk-dmo-x64.exe
```

## Imports finais do executavel

Validado com `dumpbin /imports`:

```text
mss64.dll
AIL_set_redist_directory
AIL_close_digital_driver
AIL_open_digital_driver
AIL_shutdown
AIL_startup
```

Nao importa mais:

```text
AIL_quick_startup
AIL_quick_shutdown
AIL_quick_handles
```

## Runtime atualizado

Executavel copiado:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Core Miles 9.4d:

```text
C:\0-NewDmo\ClientDist1_x64\mss64.dll
C:\0-NewDmo\ClientDist1_x64\mss64midi.dll
```

Providers ativos:

```text
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64dolby.flt
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64ds3d.flt
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64dsp.flt
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64eax.flt
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64mp3.asi
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64srs.flt
```

Todos validados como:

```text
8664 machine (x64)
```

Backup dos providers x86 removidos do diretório ativo:

```text
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\_x86_active_backup_before_mss94d_20260607_051141
```

## Smoke test

Comando:

```powershell
Start-Process `
  -FilePath "C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe" `
  -WorkingDirectory "C:\0-NewDmo\ClientDist1_x64"
```

Resultado:

```text
SMOKE_ALIVE_AFTER_20S
NO_NEW_DUMPS
```

Nao houve crash imediato de load/link/audio.

Observacao: o processo de smoke test ficou vivo em:

```text
PID 8924
```

`CloseMainWindow` foi enviado, mas `Stop-Process` e `taskkill` falharam com `Acesso negado`, provavelmente porque o executavel subiu elevado pelo manifesto. Fechar manualmente antes de substituir o exe de novo.

## Estado

Audio x64 esta novamente habilitado no source e no build.

Proxima validacao recomendada:

```text
Rodar login -> entrar no mapa -> confirmar BGM/SFX -> verificar se nao reaparece crash em mss64.dll.
```
