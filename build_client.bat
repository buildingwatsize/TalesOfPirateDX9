@echo off
setlocal
title Build Client (Game.exe)

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set PROJECT=sources\Client\proj\kop.vcxproj
set CONFIG=Release
set PLATFORM=x64

echo ========================================
echo  Building Client EXE (%CONFIG%|%PLATFORM%)
echo ========================================

%MSBUILD% %PROJECT% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 (
    echo.
    echo [FAIL] Client build failed!
    pause
    exit /b 1
)

echo.
echo [OK] Client built: sources\Client\bin\system\Game.exe
endlocal
