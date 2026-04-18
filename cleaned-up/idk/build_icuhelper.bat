@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo === Building ICUHelper x64 ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\ICUHelper\proj"
MSBuild.exe ICUHelper.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo ICU_BUILD_FAILED) else (echo ICU_BUILD_OK)
echo === DONE ===
