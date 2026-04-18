@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo === Building PAI/BTI stub for x64 ===
cd /d "C:\TalesOfPirateDX9\sources\Libraries\Ipss\src"
cl.exe /c /EHsc /W0 /DWIN32 /D_WINDOWS pai_stub.cpp /Fo:pai_stub.obj
if errorlevel 1 (echo COMPILE FAILED & exit /b 1)
lib.exe /OUT:..\lib\x64\PAI.lib pai_stub.obj
if errorlevel 1 (echo LIB FAILED & exit /b 1)
lib.exe /OUT:..\lib\x64\BTI.lib pai_stub.obj
if errorlevel 1 (echo LIB FAILED & exit /b 1)
del pai_stub.obj
echo PAI/BTI stub x64 libs created OK
dir ..\lib\x64\
