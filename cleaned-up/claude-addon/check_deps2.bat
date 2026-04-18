@echo off
set DUMP="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\dumpbin.exe"
echo === CaLua.dll deps ===
%DUMP% /DEPENDENTS C:\TalesOfPirateDX9\Client\CaLua.dll 2>&1 | findstr /i "dll DLL"
echo === PAI.dll deps ===
%DUMP% /DEPENDENTS C:\TalesOfPirateDX9\Client\PAI.dll 2>&1 | findstr /i "dll DLL"
echo === Client dir DLLs ===
dir C:\TalesOfPirateDX9\Client\*.dll /b
echo === Check new DLL exports count ===
%DUMP% /EXPORTS C:\TalesOfPirateDX9\Client\MindPower3D_D8R.dll 2>&1 | findstr "ordinal"
