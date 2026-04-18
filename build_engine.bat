@echo off
setlocal
title Build Engine DLL (MindPower3D_D8R.dll)

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set PROJECT=sources\Engine\proj\MindPower3D.vcxproj
set CONFIG=Release
set PLATFORM=x64

echo ========================================
echo  Building Engine DLL (%CONFIG%|%PLATFORM%)
echo ========================================

%MSBUILD% %PROJECT% /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 (
    echo.
    echo [FAIL] Engine build failed!
    pause
    exit /b 1
)

echo.
echo [OK] Engine built: sources\Engine\lib\MindPower3D_D8R.dll
endlocal
