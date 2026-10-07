@echo off
setlocal
if not exist "..\dist\OpenPDFOffice.exe" (echo [ERROR] Build dist first. & exit /b 1)
set ISCC=%ProgramFiles(x86)%\Inno Setup 6\ISCC.exe
if not exist "%ISCC%" (echo [ERROR] Inno Setup 6 not found. & exit /b 1)
"%ISCC%" OpenPDFOffice.iss
endlocal
