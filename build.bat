@echo off
setlocal

set "BUILD_DIR=build"

echo Checking for prerequisites...

where git >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Git is not installed. Please install Git and try again.
    exit /b 1
)

where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo CMake is not installed. Please install CMake 3.27 or newer and try again.
    exit /b 1
)

if not exist "vcpkg\bootstrap-vcpkg.bat" (
    echo Error: vcpkg bootstrap script not found. Make sure vcpkg is in the repository.
    exit /b 1
)

call vcpkg\bootstrap-vcpkg.bat

call vcpkg\vcpkg install

cmake -B "%BUILD_DIR%" -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake


endlocal
