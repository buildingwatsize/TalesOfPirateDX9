@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cd /d "C:\TalesOfPirateDX9\sources\Engine\proj"
if not exist "..\build\Release" mkdir "..\build\Release"
echo Building MindPower3D (Build, single-threaded)...
MSBuild.exe MindPower3D.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m:1 /nologo /v:minimal
if errorlevel 1 echo MINDPOWER3D FAILED
echo Done.
