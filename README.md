# IbPerfLib

A small library, which can be used to read the performance counters of all Infiniband devices in a network.

This project uses the libibmad- and libibnetdisc-libraries to automatically discover all Infiniband devices in a
network and read their performance counters.  
It can also operate in a compatibility mode in which IbPerfLib doesn't require root privileges by using the filesystem to read the performance counters. However, it will only detect local infiniband devices in this mode.

It is also possible, to read the diagnostic performance counters of local infiniband devices.

# Build instructions

This project supports Linux only. It uses the libibmad- and libibnetdisc-libraries. CMake is used to generate build
scripts. To compile everything, just run *build.sh*.  
The output will be a static  library, that can be linked against other projects.  
To let cmake automatically download and build IbPerfLib for your project, you may insert the following code into your CMakeLists.txt:

```
include(ExternalProject)

ExternalProject_Add(IbPerfLib_git
        GIT_REPOSITORY https://github.com/hhu-bsinfo/ibperf-lib.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/IbPerfLib
        INSTALL_COMMAND "")

ExternalProject_Get_Property(IbPerfLib_git binary_dir)
ExternalProject_Get_Property(IbPerfLib_git source_dir)

add_library(IbPerfLib STATIC IMPORTED)
set_property(TARGET IbPerfLib PROPERTY IMPORTED_LOCATION ${binary_dir}/ar/libIbPerfLib.a)

add_dependencies(IbPerfLib IbPerfLib_git)

include_directories(${source_dir}/src)
```

All you have left to do now, is to link your project against `IbPerfLib`, `ibverbs`, `ibmad`, and `ibnetdisc` using cmake's `target_link_libraries()` instruction. You also may need to add `/usr/include/infiniband` to your include directories.

```
target_link_libraries(IbPerfLib ibverbs ibmad ibnetdisc)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -I/usr/include/infiniband)
```


# Run instructions

IbPerfLib comes with two small test programs called *IbPerfTest* and *IbDiagPerfTest*.  
*IbPerfTest* scans your Infiniband network for devices and prints their counters to `stdout`.  
*IbDiagPerfTest* prints the diagnostic counters of your local infiniband devices.

On a Debian-based system, you can run theses commands to build and run *IbPerfTest*:

```
sudo apt install cmake libibmad-dev libibumad-dev libibnetdisc-dev libopensm-dev
./build.sh
sudo ./build/bin/IbPerfTest mad
```

If you don't have root privileges, you can run IbPerfTest in compatability mode:

```
./build/bin/IbPerfTest compat
```

To run IbDiagPerfTest, run the following command:

```
./build/bin/IbDiagPerfTest
```
