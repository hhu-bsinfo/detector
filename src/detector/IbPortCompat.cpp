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

#include <cstring>
#include <iostream>
#include "IbPortCompat.h"
#include "detector/exception/IbFileException.h"

namespace Detector {

IbPortCompat::IbPortCompat(std::string deviceName, ibv_port_attr attributes, uint8_t portNum) :
        IbPort(attributes, portNum),
        m_deviceName(std::move(deviceName)) {
    std::string path = "/sys/class/infiniband/" + m_deviceName + "/ports/" + std::to_string(m_portNum) + "/counters/";

    files[0] = std::ifstream(path + "port_xmit_data", std::ios::in);
    files[1] = std::ifstream(path + "port_rcv_data", std::ios::in);
    files[2] = std::ifstream(path + "port_xmit_packets", std::ios::in);
    files[3] = std::ifstream(path + "port_rcv_packets", std::ios::in);
    files[4] = std::ifstream(path + "unicast_xmit_packets", std::ios::in);
    files[5] = std::ifstream(path + "unicast_rcv_packets", std::ios::in);
    files[6] = std::ifstream(path + "multicast_xmit_packets", std::ios::in);
    files[7] = std::ifstream(path + "multicast_rcv_packets", std::ios::in);
    files[8] = std::ifstream(path + "symbol_error", std::ios::in);
    files[9] = std::ifstream(path + "link_downed", std::ios::in);
    files[10] = std::ifstream(path + "link_error_recovery", std::ios::in);
    files[11] = std::ifstream(path + "port_rcv_errors", std::ios::in);
    files[12] = std::ifstream(path + "port_rcv_remote_physical_errors", std::ios::in);
    files[13] = std::ifstream(path + "port_rcv_switch_relay_errors", std::ios::in);
    files[14] = std::ifstream(path + "port_xmit_discards", std::ios::in);
    files[15] = std::ifstream(path + "port_xmit_constraint_errors", std::ios::in);
    files[16] = std::ifstream(path + "port_rcv_constraint_errors", std::ios::in);
    files[17] = std::ifstream(path + "local_link_integrity_errors", std::ios::in);
    files[18] = std::ifstream(path + "excessive_buffer_overrun_errors", std::ios::in);
    files[19] = std::ifstream(path + "VL15_dropped", std::ios::in);
    files[20] = std::ifstream(path + "port_xmit_wait", std::ios::in);

    for (const std::ifstream &file : files) {
        if (!file.is_open()) {
            throw IbFileException("Unable to open file in '" + path + "'!");
        }
    }

    std::memset(baseValues, 0, sizeof(baseValues));
}

IbPortCompat::~IbPortCompat() {
    for (std::ifstream &file : files) {
        file.close();
    }
}

void IbPortCompat::ResetCounters() {
    for(uint32_t i = 0; i < sizeof(files) / sizeof(std::ifstream); i++) {
        files[i].seekg(0, std::ios::beg);
        files[i].getline(buffer, sizeof(buffer));

        baseValues[i] = strtoull(buffer, nullptr, 10);
    }
}

void IbPortCompat::RefreshCounters() {
    m_xmitDataBytes = ReadCounter(0) * m_linkWidth;
    m_rcvDataBytes = ReadCounter(1) * m_linkWidth;
    m_xmitPkts = ReadCounter(2);
    m_rcvPkts = ReadCounter(3);
    m_unicastXmitPkts = ReadCounter(4);
    m_unicastRcvPkts = ReadCounter(5);
    m_multicastXmitPkts = ReadCounter(6);
    m_multicastRcvPkts = ReadCounter(7);
    m_symbolErrors = ReadCounter(8);
    m_linkDowned = ReadCounter(9);
    m_linkRecoveries = ReadCounter(10);
    m_rcvErrors = ReadCounter(11);
    m_rcvRemotePhysicalErrors = ReadCounter(12);
    m_rcvSwitchRelayErrors = ReadCounter(13);
    m_xmitDiscards = ReadCounter(14);
    m_xmitConstraintErrors = ReadCounter(15);
    m_rcvConstraintErrors = ReadCounter(16);
    m_localLinkIntegrityErrors = ReadCounter(17);
    m_excessiveBufferOverrunErrors = ReadCounter(18);
    m_vl15Dropped = ReadCounter(19);
    m_xmitWait = ReadCounter(20);
}

uint64_t IbPortCompat::ReadCounter(uint8_t index) {
    std::memset(buffer, 0, sizeof(buffer));

    if(!files[index].good()) {
        throw IbFileException("Unable to read file!");
    }

    files[index].seekg(0, std::ios::beg);
    files[index].getline(buffer, sizeof(buffer));

    return strtoull(buffer, nullptr, 10) - baseValues[index];
}

}
