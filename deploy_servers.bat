@echo off
setlocal
title Deploy Server Binaries
cd /d "%~dp0"

echo ============================================================
echo  Deploying Server Binaries
echo ============================================================
echo.

echo Stopping servers if running...
taskkill /F /IM AccountServer.exe >nul 2>&1
taskkill /F /IM GroupServer.exe >nul 2>&1
taskkill /F /IM GateServer.exe >nul 2>&1
taskkill /F /IM GameServer.exe >nul 2>&1
timeout /t 3 /nobreak >nul

echo Copying AccountServer.exe...
copy /Y "sources\Server\AccountServer\Bin\AccountServer.exe" "server\AccountServer\AccountServer.exe"
if errorlevel 1 echo [WARN] Could not copy AccountServer.exe

echo Copying GroupServer.exe...
copy /Y "sources\Server\GroupServer\Bin\GroupServer.exe" "server\GroupServer\GroupServer.exe"
if errorlevel 1 echo [WARN] Could not copy GroupServer.exe

echo Copying GateServer.exe...
copy /Y "sources\Server\GateServer\Bin\GateServer.exe" "server\GateServer\GateServer.exe"
if errorlevel 1 echo [WARN] Could not copy GateServer.exe

echo Copying GameServer.exe...
copy /Y "sources\Server\GameServer\Bin\GameServer.exe" "server\GameServer\GameServer.exe"
if errorlevel 1 echo [WARN] Could not copy GameServer.exe

echo.
echo [OK] All server binaries deployed.
endlocal
