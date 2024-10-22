# Installation Guide

This guide will walk you through the steps needed to configure, build, and run the project on both Linux and Windows systems. Please follow the appropriate instructions for your operating system.

## Linux

### Prerequisites:
- **CMake** version 3.27 or above
- **Ninja** build system
- **Git**

### Steps:
1. **Clone the repository:**
    ```bash
    git clone https://github.com/Fizo55/R-Type.git
    cd R-Type
    ```

4. **Set up vcpkg:**
    ```bash
    ./vcpkg/bootstrap-vcpkg.sh
    ./vcpkg/vcpkg install
    ```

5. **Run CMake to configure the project: Use Ninja as the generator:**
    ```bash
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -G Ninja
    ```

6. **Build the project: Once the configuration is done, you can start the build process:**
    ```bash
    ninja -C build
    ```

7. **Run the executable: After building, run the generated executable:**
    ```bash
    ./<executable_name>
    ```

## Windows

### Prerequisites:
- **Visual Studio 2022** (Ensure C++ build tools are installed)
- ***CMake** version 3.27 or above
- **Git**
- **vcpkg** for package management

### Steps:

1. Enable Long Path Support in Git: Git might encounter long path issues on Windows. To fix this, run the following command to enable long path support in Git:
    ```bash
    git config --system core.longpaths true
    ```

2. Enable Long Path Support in Windows: Open file_path.reg as administrator then restart the computer. (Optional step if the above step haven't solved the long path issue)

3. Clone the repository: Open a terminal (PowerShell or Git Bash) and clone the repository:
    ```bash
    git clone https://github.com/Fizo55/R-Type.git
    cd R-Type
    ```

4. Set up vcpkg:
    ```bash
    .\vcpkg\bootstrap-vcpkg.bat
    .\vcpkg\vcpkg.exe install
    ```

5. Run CMake to configure the project:
    ```bash
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
    ```

6. Open the project in Visual Studio:
    ```bash
    msbuild /p:Configuration=Release server.sln
    ```

7. Run the executable: After building, you can run the executable from the build folder:
    ```bash
    ./build/<executable_name>.exe
    ```