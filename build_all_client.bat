@echo off
setlocal
title Full Client Build Pipeline
cd /d "%~dp0"

echo ============================================================
echo  Tales of Pirate DX9 - Full Client Build Pipeline
echo ============================================================
echo.

REM Step 1: Kill running game
echo [Step 1/6] Stopping Game.exe...
taskkill /F /IM Game.exe >nul 2>&1
timeout /t 2 /nobreak >nul
echo           Done.
echo.

REM Step 2: Build Libraries
echo [Step 2/6] Building Libraries (Lua, CaLua, AudioSDL)...
call build_libs.bat
if errorlevel 1 goto :fail
echo.

REM Step 3: Build Engine DLL
echo [Step 3/6] Building Engine (MindPower3D_D8R.dll)...
call build_engine.bat
if errorlevel 1 goto :fail
echo.

REM Step 4: Build Client EXE
echo [Step 4/6] Building Client (Game.exe)...
call build_client.bat
if errorlevel 1 goto :fail
echo.

REM Step 5: Deploy binaries
echo [Step 5/6] Deploying binaries...
call deploy_client.bat
if errorlevel 1 goto :fail
echo.

REM Step 6: Compile CLU and Table scripts
echo [Step 6/6] Compiling CLU and Table scripts...
call compile_scripts.bat
echo.

echo ============================================================
echo  BUILD COMPLETE - All steps succeeded!
echo ============================================================
echo.
echo  You can now launch the game:
echo    cd Client ^&^& system\Game.exe pKcfT0PcaX
echo.
pause
exit /b 0

:fail
echo.
echo ============================================================
echo  BUILD FAILED - See errors above.
echo ============================================================
pause
exit /b 1
