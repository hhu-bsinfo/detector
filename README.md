# Detector: A small library for reading the performance counters of all InfiniBand devices in a network

<p align="center">
<img src="logo.png" height=250>
</p>

<p align="center">
<a href="https://travis-ci.org/hhu-bsinfo/detector"><img src="https://travis-ci.org/hhu-bsinfo/detector.svg?branch=master"></a>
<a href="https://isocpp.org/"><img src="https://img.shields.io/badge/C%2B%2B-11-blue"></a>
<a href="https://github.com/hhu-bsinfo/observatory/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-GPLv3-orange.svg"></a>
</p>

This project uses the libibmad- and libibnetdisc-libraries to automatically discover all InfiniBand devices in a
network and read their performance counters.  
It can also operate in a compatibility mode in which it doesn't require root privileges by using the filesystem to read the performance counters. However, it will only detect local infiniBand devices in this mode.

It is also possible, to read the diagnostic performance counters of local infiniBand devices.

# Build instructions

This project supports Linux only. It uses the libibmad- and libibnetdisc-libraries. CMake is used to generate build
scripts. To compile everything, just run *build.sh*.  
The output will be a static  library, that can be linked against other projects.  
To let cmake automatically download and build Detector for your project, you may insert the following code into your CMakeLists.txt:

```
include(ExternalProject)

ExternalProject_Add(detector_git
        GIT_REPOSITORY https://github.com/hhu-bsinfo/detector.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/detector
        INSTALL_COMMAND "")

ExternalProject_Get_Property(detector_git binary_dir)
ExternalProject_Get_Property(detector_git source_dir)

add_library(detector STATIC IMPORTED)
set_property(TARGET detector PROPERTY IMPORTED_LOCATION ${binary_dir}/ar/libdetector.a)

add_dependencies(detector detector_git)

include_directories(${source_dir}/src)
```

All you have left to do now, is to link your project against `Detector`, `ibverbs`, `ibmad`, and `ibnetdisc` using cmake's `target_link_libraries()` instruction. You also may need to add `/usr/include/infiniband` to your include directories.

```
target_link_libraries(Detector ibverbs ibmad ibnetdisc)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -I/usr/include/infiniband)
```


# Run instructions

Detector comes with two small test programs called *perftest* and *diagperftest*.  
*perftest* scans your InfiniBand network for devices and prints their counters to `stdout`.  
*diagperftest* prints the diagnostic counters of your local infiniBand devices.

On a Debian-based system, you can run theses commands to build and run *perftest*:

```
sudo apt install cmake libibmad-dev libibumad-dev libibnetdisc-dev libopensm-dev
./build.sh
sudo ./build/bin/perftest network mad
```

If you don't have root privileges, you can run perftest in compatability mode:

```
./build/bin/perftest local compat
```

To run diagtest, run the following command:

```
./build/bin/diagtest
```
