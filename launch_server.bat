@echo off
setlocal
cd /d "%~dp0\server"
echo Launching Servers...
call "Connect server.bat"
endlocal
