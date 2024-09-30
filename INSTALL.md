TODO : Rewrite the INSTALL.md correctly

git submodule update --init

.\vcpkg\bootstrap-vcpkg.bat

cmake -B build -G "Ninja" -DCMAKE_CXX_COMPILER=g++ "-DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake"

ninja -C build
