#!/bin/bash

BUILD_DIR="build"

install_if_missing() {
    if ! dpkg -s "$1" >/dev/null 2>&1; then
        echo "Installing $1..."
        sudo apt-get install -y "$1"
    else
        echo "$1 is already installed."
    fi
}

echo "Checking for and installing prerequisites..."
sudo apt-get update
install_if_missing cmake
install_if_missing ninja-build
install_if_missing git
install_if_missing automake
install_if_missing libtool
install_if_missing pkg-config

CMAKE_VERSION=$(cmake --version | head -n 1 | awk '{print $3}')
REQUIRED_CMAKE_VERSION="3.27.0"
if [ "$(printf '%s\n' "$REQUIRED_CMAKE_VERSION" "$CMAKE_VERSION" | sort -V | head -n1)" != "$REQUIRED_CMAKE_VERSION" ]; then
    echo "Error: CMake version 3.27 or higher is required."
    exit 1
else
    echo "CMake version $CMAKE_VERSION is sufficient."
fi

if [ ! -f "./vcpkg/bootstrap-vcpkg.sh" ]; then
    echo "Error: vcpkg/bootstrap-vcpkg.sh not found. Make sure vcpkg is in the repository."
    exit 1
fi
./vcpkg/bootstrap-vcpkg.sh

./vcpkg/vcpkg install

cmake -B "$BUILD_DIR" "-DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake" -G Ninja

ninja -C "$BUILD_DIR"
