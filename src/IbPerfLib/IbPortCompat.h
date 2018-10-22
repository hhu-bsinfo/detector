/*
 * Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,
 * Institute of Computer Science, Department Operating Systems
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef IBPERFLIB_IBPORTCOMPAT_H
#define IBPERFLIB_IBPORTCOMPAT_H

#include <cstdint>
#include <fstream>
#include "IbPort.h"

namespace IbPerfLib {

/**
 * Implementation of IbPort, which does not use the ibmad-library to read the performance counters,
 * but instead reads the from the filesystem. This does not require root privileges,
 * but only works for local infiniband devices.
 */
class IbPortCompat : public IbPort {

public:
    /**
     * Constructor.
     *
     * @param deviceName The name of the port's device.
     * @param portNum The number, that the port has on its device.
     */
    IbPortCompat(std::string deviceName, ibv_port_attr attributes, uint8_t portNum);

    /**
     * Destructor.
     */
    ~IbPortCompat() override;

    /**
     * Overriding function from IbPort.
     */
    void ResetCounters() override;

    /**
     * Overriding function from IbPort.
     */
    void RefreshCounters() override;

private:
    /**
     * Read a single counter.
     *
     * @param index Index into files array
     */
    uint64_t ReadCounter(uint8_t index);

private:

    std::string m_deviceName;
    uint8_t m_portNum;

    char buffer[128];

    std::ifstream files[21];
    uint64_t baseValues[21];

};

}

#endif
