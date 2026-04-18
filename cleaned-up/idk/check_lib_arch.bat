@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
dumpbin /HEADERS "%1" 2>&1 | findstr /c:"machine" | findstr /v "findstr" > "%TEMP%\dumpbin_out.txt"
type "%TEMP%\dumpbin_out.txt" | findstr /n "." | findstr "^1:" 
echo FIRST_MACHINE_LINE_ABOVE
for /f %%a in ('type "%TEMP%\dumpbin_out.txt" ^| find /c "8664"') do echo X64_COUNT=%%a
for /f %%a in ('type "%TEMP%\dumpbin_out.txt" ^| find /c "14C"') do echo X86_COUNT=%%a
echo EXIT_CODE=%ERRORLEVEL%
