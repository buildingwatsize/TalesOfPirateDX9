@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo === Rebuilding logutil x64 ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\Util\src"
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" logutil.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
echo === logutil exit: %errorlevel% ===
echo === Rebuilding Game.exe x64 ===
cd /d "C:\TalesOfPirateDX9\sources\Client\proj"
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" kop.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
echo === Game.exe exit: %errorlevel% ===
