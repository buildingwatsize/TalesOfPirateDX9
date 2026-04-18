@echo off
setlocal
title Build All Servers
cd /d "%~dp0"

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set CONFIG=Release
set PLATFORM=x64

echo ============================================================
echo  Building All Server Executables (%CONFIG%|%PLATFORM%)
echo ============================================================
echo.

echo [1/4] AccountServer...
%MSBUILD% sources\Server\AccountServer\Proj\AccountServer.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] AccountServer & goto :fail )
echo       [OK]
echo.

echo [2/4] GroupServer...
%MSBUILD% sources\Server\GroupServer\Proj\GroupServer.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] GroupServer & goto :fail )
echo       [OK]
echo.

echo [3/4] GateServer...
%MSBUILD% sources\Server\GateServer\Proj\GateServer.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] GateServer & goto :fail )
echo       [OK]
echo.

echo [4/4] GameServer...
%MSBUILD% sources\Server\GameServer\Proj\GameServer.vcxproj /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m /nologo /v:minimal
if errorlevel 1 ( echo [FAIL] GameServer & goto :fail )
echo       [OK]
echo.

echo ============================================================
echo  All servers built successfully!
echo ============================================================
pause
exit /b 0

:fail
echo.
echo [FAIL] Server build pipeline failed. See errors above.
pause
exit /b 1
