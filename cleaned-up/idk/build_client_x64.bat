@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo ============================================
echo  Building ALL x64 Client Dependencies
echo ============================================

echo === 1/10 cryptlib ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\Cryptopp"
MSBuild.exe cryptlib.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo CRYPTLIB FAILED

echo === 2/10 Common ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\common\Proj"
MSBuild.exe Common.vcxproj /p:Configuration=Long_Struct_Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo COMMON FAILED

echo === 3/10 logutil ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\Util\src"
MSBuild.exe logutil.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 echo LOGUTIL FAILED

echo === 4/10 EncLib ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\EncLib\proj"
MSBuild.exe EncLib.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo ENCLIB FAILED

echo === 5/10 LIBDBC ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\Server\sdk\Proj"
MSBuild.exe LIBDBC.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo LIBDBC FAILED

echo === 6/10 Status ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\Status\Proj"
MSBuild.exe Status.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo STATUS FAILED

echo === 7/10 lua50 + lualib ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\CaLua\Lua"
MSBuild.exe lua50.vcxproj "/p:Configuration=Static Library" /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo LUA50 FAILED
MSBuild.exe lualib.vcxproj "/p:Configuration=Static Library" /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo LUALIB FAILED

echo === 8/10 CaLua ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\CaLua\CaLua"
MSBuild.exe CaLua.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo CALUA FAILED

echo === 9/10 AudioSDL ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\AudioSDL\proj"
MSBuild.exe AudioSDL.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo AUDIOSDL FAILED

echo === 10/10 ICUHelper ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\ICUHelper\proj"
MSBuild.exe ICUHelper.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo ICUHELPER FAILED

echo ============================================
echo  Building MindPower3D DLL
echo ============================================
cd /d "C:\TalesOfPirateDX9\sources\Engine\proj"
MSBuild.exe MindPower3D.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo MINDPOWER3D FAILED

echo ============================================
echo  Building Game.exe
echo ============================================
cd /d "C:\TalesOfPirateDX9\sources\Client\proj"
MSBuild.exe kop.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /m /nologo /v:minimal
if errorlevel 1 echo GAME.EXE FAILED

echo ============================================
echo  BUILD COMPLETE
echo ============================================
