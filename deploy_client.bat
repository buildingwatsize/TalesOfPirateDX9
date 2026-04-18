@echo off
setlocal
title Deploy Client Binaries

echo ========================================
echo  Deploying Client Binaries
echo ========================================

echo Killing Game.exe if running...
taskkill /F /IM Game.exe >nul 2>&1
timeout /t 2 /nobreak >nul

echo Copying Game.exe...
copy /Y "sources\Client\bin\system\Game.exe" "Client\system\Game.exe"
if errorlevel 1 (
    echo [FAIL] Could not copy Game.exe - is the game still running?
    pause
    exit /b 1
)

echo Copying MindPower3D_D8R.dll...
copy /Y "sources\Engine\lib\MindPower3D_D8R.dll" "Client\system\MindPower3D_D8R.dll"
if errorlevel 1 (
    echo [FAIL] Could not copy MindPower3D_D8R.dll
    pause
    exit /b 1
)

echo Copying CaLua.dll...
copy /Y "sources\Libraries\CaLua\lib\CaLua.dll" "Client\system\CaLua.dll" >nul 2>&1

echo.
echo [OK] All binaries deployed to Client\system\
endlocal
