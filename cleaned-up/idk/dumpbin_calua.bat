@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
dumpbin /MAP "C:\TalesOfPirateDX9\sources\Libraries\CaLua\lib\CaLua.dll" > "C:\TalesOfPirateDX9\calua_map.txt" 2>&1
echo exit: %errorlevel%
