@echo off
del C:\TalesOfPirateDX9\Client\log\*.log 2>nul
cd C:\TalesOfPirateDX9\Client
start /wait Game.exe pKcfT0PcaX
echo Game exited.
ping -n 3 127.0.0.1 > nul
wevtutil qe Application /c:5 /rd:true /f:text > C:\TalesOfPirateDX9\crash_events.log 2>&1
echo Event log saved.
