# IbPerfLib

A small library, which can be used to read the performance counters of all Infiniband devices in a network.

This project uses the libibmad- and libibnetdisc-libraries to automatically discover all Infiniband devices in a
network and read their performance counters.

# Build instructions

This project supports Linux only. It uses the libibmad- and libibnetdisc-libraries. CMake is used to generate build
scripts. To compile everything, just run *build.sh*.  
The output will be a static  library, that can be linked against other projects.  
To let cmake automatically download and build IbPerfLib for your project, you may insert the following code into your CMakeLists.txt:

```
include(ExternalProject)

ExternalProject_Add(IbPerfLib_git
        GIT_REPOSITORY git@git.hhu.de:faruh100/IbPerfLib.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/IbPerfLib
        INSTALL_COMMAND "")

ExternalProject_Get_Property(IbPerfLib_git binary_dir)
ExternalProject_Get_Property(IbPerfLib_git source_dir)

add_library(IbPerfLib STATIC IMPORTED)
set_property(TARGET IbPerfLib PROPERTY IMPORTED_LOCATION ${binary_dir}/ar/libIbPerfLib.a)

add_dependencies(IbPerfLib IbPerfLib_git)

include_directories(${source_dir}/src)

```

All you have left to do now, is to link your project against `IbPerfLib` using cmake's `target_link_libraries()` instruction.


# Run instructions

IbPerfLib comes with a small test program called *IbPerfTest*, which scans your Infiniband network for devices and prints their counters to `stdout`.

On a Debian-based system, you can run theses commands to build and run *IbPerfTest*:

```
sudo apt install cmake libibmad-dev libibumad-dev libibnetdisc-dev libopensm-dev
./build.sh
sudo ./build/bin/IbPerfTest
```