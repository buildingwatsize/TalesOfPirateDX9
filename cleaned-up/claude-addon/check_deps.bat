@echo off
echo === MindPower3D_D8R.dll imports ===
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe" /DEPENDENTS C:\TalesOfPirateDX9\Client\MindPower3D_D8R.dll > C:\TalesOfPirateDX9\deps_dll.log 2>&1
type C:\TalesOfPirateDX9\deps_dll.log
echo.
echo === Game.exe imports ===
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe" /DEPENDENTS C:\TalesOfPirateDX9\Client\Game.exe > C:\TalesOfPirateDX9\deps_game.log 2>&1
type C:\TalesOfPirateDX9\deps_game.log
