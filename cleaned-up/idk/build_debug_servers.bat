@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo === Building AccountServer ===
cd /d "C:\TalesOfPirateDX9\sources\Server\AccountServer\Proj"
MSBuild.exe AccountServer.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo ACCOUNTSERVER BUILD FAILED) else (echo ACCOUNTSERVER BUILD OK)

echo === Building GateServer ===
cd /d "C:\TalesOfPirateDX9\sources\Server\GateServer\Proj"
MSBuild.exe GateServer.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo GATESERVER BUILD FAILED) else (echo GATESERVER BUILD OK)

echo === Building GroupServer ===
cd /d "C:\TalesOfPirateDX9\sources\Server\GroupServer\Proj"
MSBuild.exe GroupServer.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo GROUPSERVER BUILD FAILED) else (echo GROUPSERVER BUILD OK)

echo === Building GameServer ===
cd /d "C:\TalesOfPirateDX9\sources\Server\GameServer\Proj"
MSBuild.exe GameServer.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /nologo /v:minimal
if errorlevel 1 (echo GAMESERVER BUILD FAILED) else (echo GAMESERVER BUILD OK)

echo === Copying ===
copy /Y "C:\TalesOfPirateDX9\sources\Server\AccountServer\Bin\AccountServer.exe" "C:\TalesOfPirateDX9\server\AccountServer\AccountServer.exe"
copy /Y "C:\TalesOfPirateDX9\sources\Server\GateServer\Bin\GateServer.exe" "C:\TalesOfPirateDX9\server\GateServer\GateServer.exe"
copy /Y "C:\TalesOfPirateDX9\sources\Server\GroupServer\Bin\GroupServer.exe" "C:\TalesOfPirateDX9\server\GroupServer\GroupServer.exe"
copy /Y "C:\TalesOfPirateDX9\sources\Server\GameServer\Bin\GameServer.exe" "C:\TalesOfPirateDX9\server\GameServer\GameServer.exe"
echo === DONE ===
