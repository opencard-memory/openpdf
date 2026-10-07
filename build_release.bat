@echo off
setlocal
where cmake >nul 2>nul || (echo [ERROR] CMake not found. Install Visual Studio C++ and CMake. & exit /b 1)
if "%Qt6_DIR%"=="" echo [INFO] If Qt is not found, set Qt6_DIR to Qt\6.x.x\msvc2022_64\lib\cmake\Qt6
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 exit /b 1
cmake --build build --config Release
if errorlevel 1 exit /b 1
if not exist dist mkdir dist
copy /Y build\Release\OpenPDFOffice.exe dist\OpenPDFOffice.exe
where windeployqt >nul 2>nul && windeployqt --release --dir dist dist\OpenPDFOffice.exe
if exist "%Qt6_DIR%\..\..\..\bin\windeployqt.exe" "%Qt6_DIR%\..\..\..\bin\windeployqt.exe" --release --dir dist dist\OpenPDFOffice.exe
echo.
echo Build complete: dist\OpenPDFOffice.exe
endlocal
