@echo off
setlocal
title Compile CLU and Table Scripts

echo ========================================
echo  Compiling CLU Scripts (.clu -> .bin)
echo ========================================
pushd Client
start /wait system\Game.exe pKcfT0PcaX clu_bin
echo [OK] CLU compilation done.

echo.
echo ========================================
echo  Compiling Table Scripts
echo ========================================
start /wait system\Game.exe pKcfT0PcaX table_bin
echo [OK] Table compilation done.
popd

echo.
echo [OK] All scripts compiled.
endlocal
