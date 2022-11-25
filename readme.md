`p1-playground`
========

1. [Building on Windows (CLion)](#buildclion)

Building on Windows with `Conan`, `CLion`, `MinGW` & `CMake` <a name="buildclion"></a>
-------------
Step-by-step guide to building on Windows with a pre-existing CLion setup.
- Download and install the Conan package manager
  - Ensure that Conan is added to path during installation process
  - [Download Conan - C/C++ Open Source Package Manager](https://conan.io/downloads.html)
- Add `CMake` and `MinGW` to path (environment variables):

  ([Guide: Add to the PATH on Windows 10](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/))
  - Find CLion installation folder, e.g.: `C:\Program Files\JetBrains\CLion 2022.2.3`
  - Edit the following lines and add to path:
    ```
    <CLION_INSTALLATION_FOLDER>\bin\mingw\bin
    <CLION_INSTALLATION_FOLDER>\bin\cmake\bin\win
    ```
- Before continuing, open a new terminal and check that the following commands execute properly:
  ```
  conan -v
  gcc -v
  cmake -version
  ```
 - Create Conan profile for MingW 
   - Navigate to `%USERPROFILE%/.conan/profiles` (Use Run box for quick navigation: Windows Key + R)
   - Create a new file with the name ``mingw64`` with the following contents (Keep it open for later):
   ```asm
    [build_requires]
    [settings]
    os=Windows
    os_build=Windows
    arch=x86_64
    arch_build=x86_64
    compiler=gcc
    compiler.version=11.2
    compiler.libcxx=libstdc++11
    build_type=Debug
    [options]
    [env]
   ```
 - Open project in CLion
   - Click the `Terminal` button (in bottom menu)
   - Execute the following commands
     ```
     cd .\cmake-build-debug\
     conan install .. --build=missing --profile=mingw64
     ```
   - In case of version mismatch, update `compiler.version` in the profile file