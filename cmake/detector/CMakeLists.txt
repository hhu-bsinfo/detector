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

project(detector)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${DETECTOR_SRC_DIR})
 
set(SOURCE_FILES
        ${DETECTOR_SRC_DIR}/detector/BuildConfig.cpp
        ${DETECTOR_SRC_DIR}/detector/IbPerfCounter.cpp
        ${DETECTOR_SRC_DIR}/detector/IbPort.cpp
        ${DETECTOR_SRC_DIR}/detector/IbNode.cpp
        ${DETECTOR_SRC_DIR}/detector/IbFabric.cpp
        ${DETECTOR_SRC_DIR}/detector/IbDiagPerfCounter.cpp
        ${DETECTOR_SRC_DIR}/detector/IbPortCompat.cpp)
 
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/infiniband")

target_link_libraries(${PROJECT_NAME} ibverbs ibmad ibnetdisc)