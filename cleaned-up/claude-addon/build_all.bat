@echo off
echo === Rebuilding MindPower3D Engine DLL (full) ===
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "C:\TalesOfPirateDX9\sources\Engine\proj\MindPower3D.vcxproj" /p:Configuration=Release /p:Platform=Win32 /t:Rebuild /v:minimal > C:\TalesOfPirateDX9\build_engine2.log 2>&1
if errorlevel 1 ( echo ENGINE FAILED & type C:\TalesOfPirateDX9\build_engine2.log & goto :end )
echo ENGINE OK

echo === Copying Engine DLL to Client ===
copy C:\TalesOfPirateDX9\sources\Engine\lib\MindPower3D_D8R.dll C:\TalesOfPirateDX9\Client\MindPower3D_D8R.dll
echo DLL COPIED

echo === Building Game.exe (Client, incremental) ===
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "C:\TalesOfPirateDX9\sources\Client\proj\kop.vcxproj" /p:Configuration=Release /p:Platform=Win32 /t:Build /v:minimal > C:\TalesOfPirateDX9\build_client.log 2>&1
if errorlevel 1 ( echo CLIENT FAILED & type C:\TalesOfPirateDX9\build_client.log & goto :end )
echo CLIENT OK

echo === Copying Game.exe to Client ===
copy C:\TalesOfPirateDX9\sources\Client\bin\system\Game.exe C:\TalesOfPirateDX9\Client\Game.exe
echo GAME.EXE COPIED

:end
echo === DONE ===
