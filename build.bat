@echo off
setlocal

set "REPO_URL=https://github.com/Fizo55/R-Type.git"
set "REPO_DIR=R-Type"
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

where ninja >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Ninja is not installed. Please install Ninja and try again.
    exit /b 1
)

if not exist "%REPO_DIR%" (
    echo Cloning the repository...
    git clone --recursive "%REPO_URL%"
) else (
    echo Repository already cloned.
)

cd "%REPO_DIR%" || exit /b 1

if not exist "%BUILD_DIR%" (
    echo Creating build directory...
    mkdir "%BUILD_DIR%"
)

cd "%BUILD_DIR%" || exit /b 1

if not exist "vcpkg\bootstrap-vcpkg.bat" (
    echo Error: vcpkg bootstrap script not found. Make sure vcpkg is in the repository.
    exit /b 1
)

echo Bootstrapping vcpkg...
call vcpkg\bootstrap-vcpkg.bat

echo Installing dependencies with vcpkg...
call vcpkg\vcpkg install

echo Configuring the project with CMake...
cmake -B "%BUILD_DIR%" -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake -G Ninja

echo Building the project...
ninja -C "%BUILD_DIR%"


endlocal
