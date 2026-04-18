@echo off
del C:\TalesOfPirateDX9\Client\log\*.log 2>nul
cd C:\TalesOfPirateDX9\Client
start Game.exe pKcfT0PcaX
echo Game launched, waiting 12 seconds...
ping -n 13 127.0.0.1 > nul
echo === init.log ===
if exist log\init.log ( type log\init.log ) else ( echo NO_INIT_LOG )
echo === log dir ===
dir log\
