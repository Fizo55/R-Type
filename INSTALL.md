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

2. **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3. **Run CMake to configure the project: Use Ninja as the generator:**
    ```bash
    cmake -G Ninja ..
    ```

4. **Build the project: Once the configuration is done, you can start the build process:**
    ```bash
    ninja
    ```

5. **Run the executable: After building, run the generated executable:**
    ```bash
    ./<executable_name>
    ```

## Windows

### Prerequisites:
- **Visual Studio 2022** (Ensure C++ build tools are installed)
- ***CMake** version 3.27 or above
- **Git**

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

4. Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

5. Run CMake to configure the project: Use Visual Studio 17 2022 as the generator:
    ```bash
    cmake -G "Visual Studio 17 2022" ..
    ```

6. Open the solution in Visual Studio: You can now open the generated .sln file with Visual Studio or build directly from the command line using:
    ```bash
    msbuild /p:Configuration=Release server.sln
    ```

7. Run the executable: After building, you can run the executable from the build folder:
    ```bash
    ./<executable_name>.exe
    ```