@echo off
set DUMP="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe"
echo === Game.exe imports FROM MindPower3D_D8R.dll ===
%DUMP% /IMPORTS C:\TalesOfPirateDX9\Client\system\Game.exe > C:\TalesOfPirateDX9\imports_game.log 2>&1
findstr "MindPower3D" C:\TalesOfPirateDX9\imports_game.log
echo.
echo === New DLL exports (count) ===
%DUMP% /EXPORTS C:\TalesOfPirateDX9\Client\system\MindPower3D_D8R.dll > C:\TalesOfPirateDX9\exports_dll.log 2>&1
findstr "number of" C:\TalesOfPirateDX9\exports_dll.log
echo.
echo === Symlinks in Client root ===
dir C:\TalesOfPirateDX9\Client\ /al 2>&1
