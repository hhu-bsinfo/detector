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

#ifndef DETECTOR_IBPORTCOUNTER_H
#define DETECTOR_IBPORTCOUNTER_H

// The extended 64-bit versions of rcvErrors, rcvRemotePhysicalErrors, rcvSwitchRelayErrors, xmitDiscards,
// xmitConstraintErrors, rcvConstraintErrors, localLinkIntegrityErrors, excessiveBufferOverrunErrors, xmitWait
// are only available in new versions of libibmad. By default build script checks, if the counters are availabe.
#ifndef USE_ADDITIONAL_EXTENDED_COUNTERS
#define USE_ADDITIONAL_EXTENDED_COUNTERS 0
#endif

#define DEFAULT_QUERY_TIMEOUT 0
#define QUERY_BUF_SIZE 1536
#define RESET_BUF_SIZE 1024

#ifndef IB_PM_IS_ADDL_PORT_CTRS_EXT_SUP
#define IB_PM_IS_ADDL_PORT_CTRS_EXT_SUP (CL_HTON32(((uint32_t)1)<<1))
#endif

#include <cstdint>
#include <iostream>
#include <infiniband/mad.h>
#include <infiniband/iba/ib_types.h>
#include <infiniband/verbs.h>
#include "IbPerfCounter.h"

namespace Detector {

/**
 * Reads performance counters from a single port of an InfiniBand device.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class IbPort : public IbPerfCounter {

public:
    /**
     * Constructor.
     *
     * @param lid The port's local id
     * @param portNum The number, that the port has on its device
     */
    IbPort(uint16_t lid, uint8_t portNum);

    /**
     * Destructor.
     */
    ~IbPort() override;

    /**
     * Reset all MAD-counters.
     */
    void ResetCounters() override;

    /**
     * Query all MAD-counters from all ports and aggregate the results.
     * The resulting values will be saved in the counter variables.
     */
    void RefreshCounters() override;

    /**
     * Get the port's local id.
     */
    uint16_t GetLid() const {
        return m_lid;
    }

    /**
     * Get the number, that the port has on its device.
     */
    uint8_t GetNum() const {
        return m_portNum;
    }

    /**
     * Get the port's link width.
     */
    uint8_t GetLinkWidth() const {
        return m_linkWidth;
    }

    /**
     * Write port information to an output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const IbPort &o) {
        return os
                << "Port Number: " << unsigned(o.m_portNum) << ", "
                << "LID: " << std::hex << o.m_lid << ", "
                << "Link width: " << std::dec << unsigned(o.m_linkWidth) << "x, "
                << "XmitBytes: " << o.GetXmitDataBytes() << " Bytes, "
                << "RcvBytes: " << o.GetRcvDataBytes() << " Bytes";
    }

private:
    /**
     * Calculate the real link witdh by using the active_width from ibv_port_attr.
     *
     * @param The value active_width from an ibv_port_attr_struct.
     */
    uint8_t CalcLinkWidth(uint8_t activeWidth);

protected:
    /**
     * Compatibility constructor.
     *
     * @param attributes The port's attributes
     * @param portNum The number, that the port has on its device
     */
    IbPort(ibv_port_attr attributes, uint8_t portNum);

protected:
    /**
     * The lid of the port, that shall be monitored.
     */
    uint16_t m_lid;

    /**
     * The number, that the port has on its device.
     */
    uint8_t m_portNum;

    /**
     * The port's active link width.
     */
    uint8_t m_linkWidth;

private:
    /**
     * Pointer to a mad-port. A port can be opened by calling mad_rpc_open_port().
     * A port must be opened before it is possible to query any data from the InfiniBand device.
     */
    ibmad_port *m_madPort;

    /**
     * Contains information about an InfiniBand device/port. This struct can be initialized by calling ib_portid_set().
     */
    ib_portid_t m_portId;

    /**
     * Indicates, whether this port is part of a NIC, a switch, or a router.
     */
    MAD_NODE_TYPE m_nodeType;

    /**
     * Indicates, whether or not the InfiniBand device supports the extended counters for Uni- and Multicast-operations.
     */
    bool m_isExtendedWidthSupported;

    /**
     * Indicates, whether or not the InfiniBand device supports the extended error counters.
     */
    bool m_isAdditionalExtendedPortCountersSupported;

    /**
     * Indicates, whether or not the InfiniBand device supports the transmission-wait counter.
     */
    bool m_isXmitWaitSupported;
};

}
#endif
