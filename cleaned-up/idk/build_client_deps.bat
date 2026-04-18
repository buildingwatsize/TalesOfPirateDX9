@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

echo === Building lua50 x86 ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\CaLua\Lua"
MSBuild.exe lua50.vcxproj "/p:Configuration=Static Library" /p:Platform=Win32 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo LUA50 BUILD FAILED & goto :end) else (echo LUA50 BUILD OK)

echo === Building lualib x86 ===
MSBuild.exe lualib.vcxproj "/p:Configuration=Static Library" /p:Platform=Win32 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo LUALIB BUILD FAILED & goto :end) else (echo LUALIB BUILD OK)

echo === Building CaLua x86 ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\CaLua\CaLua"
MSBuild.exe CaLua.vcxproj /p:Configuration=Release /p:Platform=Win32 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo CALUA BUILD FAILED & goto :end) else (echo CALUA BUILD OK)

echo === Building ICUHelper x86 ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\ICUHelper\proj"
MSBuild.exe ICUHelper.vcxproj /p:Configuration=Release /p:Platform=Win32 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo ICUHELPER BUILD FAILED & goto :end) else (echo ICUHELPER BUILD OK)

echo === ALL DEPS OK ===
:end
echo === DONE ===
