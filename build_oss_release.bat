@echo off
setlocal
if "%VCPKG_ROOT%"=="" (echo [ERROR] Set VCPKG_ROOT first. & exit /b 1)
if "%Qt6_DIR%"=="" (echo [ERROR] Set Qt6_DIR first. & exit /b 1)
"%VCPKG_ROOT%\vcpkg.exe" install --triplet x64-windows
if errorlevel 1 exit /b 1
set PATH=%VCPKG_ROOT%\installed\x64-windows\tools\qpdf;%PATH%
cmake -S . -B build-oss -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -DQt6_DIR="%Qt6_DIR%"
if errorlevel 1 exit /b 1
cmake --build build-oss --config Release
endlocal
