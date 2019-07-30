# Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,
# Institute of Computer Science, Department Operating Systems
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

cmake_minimum_required(VERSION 3.5)

# Set version
execute_process(COMMAND git describe --tags --abbrev=0 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE DETECTOR_VERSION ERROR_QUIET)
string(STRIP "${DETECTOR_VERSION}" DETECTOR_VERSION)
if (DETECTOR_VERSION STREQUAL "")
    set(DETECTOR_VERSION "v0.0.0")
endif ()

# Set git revision
execute_process(COMMAND git log -1 --format=%h --date=short HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE DETECTOR_GIT_REV ERROR_QUIET)
string(STRIP "${DETECTOR_GIT_REV}" DETECTOR_GIT_REV)
if (DETECTOR_GIT_REV STREQUAL "")
    set(DETECTOR_GIT_REV "not available")
endif ()

# Set git branch
execute_process(COMMAND git rev-parse --symbolic-full-name --abbrev-ref HEAD
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE DETECTOR_GIT_BRANCH ERROR_QUIET)
string(STRIP "${DETECTOR_GIT_BRANCH}" DETECTOR_GIT_BRANCH)
if (DETECTOR_GIT_BRANCH STREQUAL "")
    set(DETECTOR_GIT_BRANCH "master")
endif ()

# Set build date
string(TIMESTAMP DETECTOR_BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Check if the installed version of the ibmad-library supports all 64-bit error counters
execute_process(COMMAND bash -c "./check_addl_ext_counters_support.sh"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE DETECTOR_USE_ADDITIONAL_EXTENDED_COUNTERS)

# Print build information
message("Detector ${DETECTOR_VERSION} - git ${DETECTOR_GIT_REV}(${DETECTOR_GIT_BRANCH}), \
build date: ${DETECTOR_BUILD_DATE}")

if (DETECTOR_USE_ADDITIONAL_EXTENDED_COUNTERS STREQUAL "1")
    message(STATUS "Additional extended counters are supported!")
else ()
    message(STATUS "Additional extended counters are not supported!")
endif ()

# Set preprocessor defines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDETECTOR_VERSION='${DETECTOR_VERSION}'\
    -DDETECTOR_GIT_REV='${DETECTOR_GIT_REV}' -DDETECTOR_GIT_BRANCH='${DETECTOR_GIT_BRANCH}' \
    -DDETECTOR_BUILD_DATE='${DETECTOR_BUILD_DATE}' \
    -DUSE_ADDITIONAL_EXTENDED_COUNTERS='${DETECTOR_USE_ADDITIONAL_EXTENDED_COUNTERS}'")

# Set source directory variables
set(DETECTOR_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
set(DETECTOR_SRC_DIR "${DETECTOR_ROOT_DIR}/src")

# Set output directory variables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Force out of source build
message(STATUS "Force out of source build check...")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
if (insource)
    message(FATAL_ERROR "Please do not build in your source dir. Use a dedicated build folder!")
endif ()

# Set compiler-flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fPIC")

# Add subdirectories
add_subdirectory(detector)
add_subdirectory(perftest)
add_subdirectory(diagtest)
