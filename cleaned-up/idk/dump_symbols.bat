@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
echo === Game.exe MAP ===
dumpbin /map "C:\TalesOfPirateDX9\sources\Client\bin\system\Game.exe" > "C:\TalesOfPirateDX9\game_map.txt" 2>&1
echo MAP done (exit: %errorlevel%)
echo === MindPower3D MAP ===
dumpbin /map "C:\TalesOfPirateDX9\sources\Engine\lib\MindPower3D_D8R.dll" > "C:\TalesOfPirateDX9\mp3d_map.txt" 2>&1
echo MAP done (exit: %errorlevel%)
