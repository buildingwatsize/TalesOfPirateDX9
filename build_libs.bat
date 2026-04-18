@echo off
setlocal
title Build Libraries (Lua, CaLua, AudioSDL)
cd /d "%~dp0"

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set CONFIG=Release
set PLATFORM=x64

echo ========================================
echo  Building Libraries (%CONFIG%|%PLATFORM%)
echo ========================================
echo.

echo [1/3] lua50 (Lua core)...
%MSBUILD% sources\Libraries\CaLua\Lua\lua50.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] lua50 & goto :fail )
echo       [OK]
echo.

echo [2/3] CaLua (Lua binding)...
%MSBUILD% sources\Libraries\CaLua\CaLua\CaLua.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] CaLua & goto :fail )
echo       [OK]
echo.

echo [3/3] AudioSDL...
%MSBUILD% sources\Libraries\AudioSDL\proj\AudioSDL.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] AudioSDL & goto :fail )
echo       [OK]
echo.

echo ========================================
echo  All libraries built successfully!
echo ========================================
endlocal
exit /b 0

:fail
echo.
echo [FAIL] Library build failed. See errors above.
pause
exit /b 1
