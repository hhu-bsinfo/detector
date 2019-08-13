# Detector: A small library for reading the performance counters of all InfiniBand devices in a network

<p align="center">
<img src="logo.svg" height=175>
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

This project supports Linux only. It uses the ibmad- and ibnetdisc-libraries. CMake is used to generate build
scripts. To compile everything, just run *build.sh*.  
The output will be a static  library, that can be linked against other projects.

# Include in other projects

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

# Usage

Using Detector is very simple and does not require a lot of code. To scan for InfiniBand devices, you just need to create an instance of `IbFabric`:

```
bool network = true;
bool compatibility = true;
Detector::IbFabric *fabric = nullptr;

try {
    fabric = new Detector::IbFabric(network, compat);
} catch(const Detector::IbPerfException &exception) {
    printf("An exception occurred: %s", exception.what());
}
```

The parameters `network` and `compatibility` determine how your network will be scanned. See [Run instructions](#run-instructions
) for more detail on these parameters.

`IbFabric` provides an array of nodes, that have been found in your network, while each node has an array of ports. To print out the amount of transmitted bytes of each port, you can use the following code:

```
for(Detector::IbNode *node : fabric.GetNodes()) {
    for(Detector::IbPort *port : node->GetPorts()) {
        printf("%s (port %u): %lu", node->GetDescription().c_str(), port->GetNum(), port->GetXmitDataBytes());
    }
}
```

This will output a string like that for every device:

```
mlx4_0 (port 1): 25672032 Bytes
```

It is also possible to get aggregated counters for a whole node, by calling the getter-methods on an `IbNode` object.

To refresh or reset the counters of a node or port, you can call `RefreshCounters()` or `ResetCounters()` respectively.  
It is also possible to refresh/reset the counters of the whole fabric at once.

# Run instructions

Detector comes with two small test programs called *perftest* and *diagtest*.  

*perftest* scans your InfiniBand network for devices and prints their counters to `stdout`, while *diagtest* prints the diagnostic counters of your local infiniBand devices.

On a Debian-based system, you can use theses commands to build and run *perftest*:

```
sudo apt install cmake libibverbs-dev libibmad-dev libibumad-dev libibnetdisc-dev libopensm-dev
./build.sh
sudo ./build/bin/perftest network mad
```

Since Ubuntu 18.04 (or Debian 10) you do not need to install the InfiniBand libraries separately, as they are provided by the package *rdma-core*.
If you are using such an operating system, you should replace the first line by the following:

```
sudo apt install cmake rdma-core
```

*perftest* can either scan your entire network or only your local machine. This is determined by the first parameter (`network`/`local`).  
The second parameter determines, whether to use the ibmad-library or run in compatibility mode (`mad`/`compat`).

In compatibility mode, the performance counters are read using the filesystem. In contrary to `mad`, this mode does not require root privileges, however it will only work for local InfiniBand devices.

```
./build/bin/perftest local compat
```

To run *diagtest*, use the following command:

```
./build/bin/diagtest
```
