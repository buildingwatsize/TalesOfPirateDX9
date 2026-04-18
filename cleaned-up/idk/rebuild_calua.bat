@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

echo === Rebuilding lua50.lib x64 ===
%MSBUILD% "C:\TalesOfPirateDX9\sources\Libraries\CaLua\Lua\lua50.vcxproj" "/p:Configuration=Static Library" /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
echo === lua50 exit: %errorlevel% ===

echo === Rebuilding lualib.lib x64 ===
%MSBUILD% "C:\TalesOfPirateDX9\sources\Libraries\CaLua\Lua\lualib.vcxproj" "/p:Configuration=Static Library" /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
echo === lualib exit: %errorlevel% ===

echo === Rebuilding CaLua.dll x64 ===
%MSBUILD% "C:\TalesOfPirateDX9\sources\Libraries\CaLua\CaLua\CaLua.vcxproj" /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
echo === CaLua exit: %errorlevel% ===
