@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo === Building Game.exe x64 with MAP ===
cd /d "C:\TalesOfPirateDX9\sources\Client\proj"
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" kop.vcxproj /p:Configuration=Release /p:Platform=x64 /p:LinkGenerateMapFile=true /t:Rebuild /m /nologo /v:minimal
echo === MSBuild exit code: %errorlevel% ===
echo === Building MindPower3D x64 with MAP ===
cd /d "C:\TalesOfPirateDX9\sources\Engine\proj"
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" MindPower3D.vcxproj /p:Configuration=Release /p:Platform=x64 /p:LinkGenerateMapFile=true /t:Rebuild /m /nologo /v:minimal
echo === MSBuild exit code: %errorlevel% ===
