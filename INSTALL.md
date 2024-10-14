TODO : Rewrite the INSTALL.md correctly

git submodule update --init

.\vcpkg\bootstrap-vcpkg.bat

mkdir build

cd build

cmake .. .

ninja -C build
