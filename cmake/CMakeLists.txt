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
        OUTPUT_VARIABLE IBPERFLIB_VERSION ERROR_QUIET)
string(STRIP "${IBPERFLIB_VERSION}" IBPERFLIB_VERSION)
if (IBPERFLIB_VERSION STREQUAL "")
    set(IBPERFLIB_VERSION "v0.0.0")
endif ()

# Set git revision
execute_process(COMMAND git log -1 --format=%h --date=short HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE IBPERFLIB_GIT_REV ERROR_QUIET)
string(STRIP "${IBPERFLIB_GIT_REV}" IBPERFLIB_GIT_REV)
if (IBPERFLIB_GIT_REV STREQUAL "")
    set(IBPERFLIB_GIT_REV "not available")
endif ()

# Set build date
string(TIMESTAMP IBPERFLIB_BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Print build information
message("IbPerfLib ${IBPERFLIB_VERSION} - git ${IBPERFLIB_GIT_REV}, build date: ${IBPERFLIB_BUILD_DATE}")

# Set preprocessor defines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIBPERFLIB_VERSION='${IBPERFLIB_VERSION}'\
    -DIBPERFLIB_GIT_REV='${IBPERFLIB_GIT_REV}' -DIBPERFLIB_BUILD_DATE='${IBPERFLIB_BUILD_DATE}'")

# Set source directory variables
set(IBPERFLIB_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
set(IBPERFLIB_SRC_DIR "${IBPERFLIB_ROOT_DIR}/src")

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fPIC")

# Add subdirectories
add_subdirectory(IbPerfLib)
add_subdirectory(IbPerfTest)
add_subdirectory(IbDiagPerfTest)