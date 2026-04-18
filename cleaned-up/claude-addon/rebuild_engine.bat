@echo off
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "C:\TalesOfPirateDX9\sources\Engine\proj\MindPower3D.vcxproj" /p:Configuration=Release /p:Platform=Win32 /t:Rebuild /v:minimal
echo EXIT_CODE=%ERRORLEVEL%
