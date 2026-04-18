@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo ============================================
echo  Building MindPower3D DLL (Release x64)
echo ============================================
cd /d "C:\TalesOfPirateDX9\sources\Engine\proj"
MSBuild.exe MindPower3D.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 echo MINDPOWER3D FAILED

echo ============================================
echo  Building Game.exe (Release x64)
echo ============================================
cd /d "C:\TalesOfPirateDX9\sources\Client\proj"
MSBuild.exe kop.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo GAME.EXE FAILED

echo ============================================
echo  BUILD COMPLETE
echo ============================================
