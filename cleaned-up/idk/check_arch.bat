@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
echo === SDL.lib ===
dumpbin /headers "C:\TalesOfPirateDX9\sources\Libraries\SDL-1.2.7\LIB\SDL.lib" 2>&1 | findstr /i "machine"
echo === PAI.lib ===
dumpbin /headers "C:\TalesOfPirateDX9\sources\Libraries\Ipss\lib\PAI.lib" 2>&1 | findstr /i "machine"
echo === discord-rpc.lib ===
dumpbin /headers "C:\TalesOfPirateDX9\sources\Libraries\Discord\lib\discord-rpc.lib" 2>&1 | findstr /i "machine"
echo === d3d9.lib ===
dumpbin /headers "C:\TalesOfPirateDX9\sources\Libraries\DirectX\DX9\Lib\d3d9.lib" 2>&1 | findstr /i "machine"
echo === ddraw (DX include) ===
dumpbin /headers "C:\TalesOfPirateDX9\sources\Libraries\DirectX\DX9\Lib\dxguid.lib" 2>&1 | findstr /i "machine"
