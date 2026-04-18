@echo off
ping -n 10 127.0.0.1 > nul
dir C:\TalesOfPirateDX9\Client\log\
echo ---
if exist C:\TalesOfPirateDX9\Client\log\init.log (
    type C:\TalesOfPirateDX9\Client\log\init.log
) else (
    echo NO_INIT_LOG
)
