@echo off
wevtutil qe Application /c:10 /rd:true /f:text > C:\TalesOfPirateDX9\events.log 2>&1
echo done
