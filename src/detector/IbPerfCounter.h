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

#ifndef DETECTOR_IBPERFCOUNTER_H
#define DETECTOR_IBPERFCOUNTER_H

#include <cstdint>
#include <ostream>

namespace Detector {

/**
 * Base class for an implementation that read MAD performance values.
 *
 * DEVELOPER NOTE: The values for XmitData and RcvData are returned per link by the MAD-library.
 *                 An implementation of IbPerfCounter should be aware of this and multiply the
 *                 these values by the port's link width.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class IbPerfCounter {

public:
    /**
     * Constructor.
     */
    IbPerfCounter();

    /**
     * Destructor.
     */
    virtual ~IbPerfCounter() = default;

    /**
     * Reset all MAD-counters.
     */
    virtual void ResetCounters() = 0;

    /**
     * Query all MAD-counters from all ports and aggregate the results.
     * The resulting values will be saved in the counter variables.
     */
    virtual void RefreshCounters() = 0;

    /**
     * Get the amount of transmitted data.
     */
    uint64_t GetXmitDataBytes() const {
        return m_xmitDataBytes;
    }

    /**
     * Get the amount of received data.
     */
    uint64_t GetRcvDataBytes() const {
        return m_rcvDataBytes;
    }

    /**
     * Get the amount of transmitted packets.
     */
    uint64_t GetXmitPkts() const {
        return m_xmitPkts;
    }

    /**
     * Get the amount of received packets.
     */
    uint64_t GetRcvPkts() const {
        return m_rcvPkts;
    }

    /**
     * Get the amount of transmitted unicast packets.
     */
    uint64_t GetUnicastXmitPkts() const {
        return m_unicastXmitPkts;
    }

    /**
     * Get the amount of received unicast packets.
     */
    uint64_t GetUnicastRcvPkts() const {
        return m_unicastRcvPkts;
    }

    /**
     * Get the amount of transmitted multicast packets.
     */
    uint64_t GetMulticastXmitPkts() const {
        return m_multicastXmitPkts;
    }

    /**
     * Get the amount of received multicast packets.
     */
    uint64_t GetMulticastRcvPkts() const {
        return m_multicastRcvPkts;
    }

    /**
     * Get the symbol-error counter.
     */
    uint64_t GetSymbolErrors() const {
        return m_symbolErrors;
    }

    /**
     * Get the Link-downed counter.
     */
    uint64_t GetLinkDownedCounter() const {
        return m_linkDowned;
    }

    /**
     * Get the Link-recovery counter.
     */
    uint64_t GetLinkRecoveryCounter() const {
        return m_linkRecoveries;
    }

    /**
     * Get the receive-error counter.
     */
    uint64_t GetRcvErrors() const {
        return m_rcvErrors;
    }

    /**
     * Get the receive-error counter for physical errors.
     */
    uint64_t GetRcvRemotePhysicalErrors() const {
        return m_rcvRemotePhysicalErrors;
    }

    /**
     * Get the receive-error counter for switch errors.
     */
    uint64_t GetRcvSwitchRelayErrors() const {
        return m_rcvSwitchRelayErrors;
    }

    /**
     * Get the amount of discarded transmissions.
     */
    uint64_t GetXmitDiscards() const {
        return m_xmitDiscards;
    }

    /**
     * Get the transmit-error counter for constraint errors.
     */
    uint64_t GetXmitConstraintErrors() const {
        return m_xmitConstraintErrors;
    }

    /**
     * Get the receive-error counter for constraint errors.
     */
    uint64_t GetRcvConstraintErrors() const {
        return m_rcvConstraintErrors;
    }

    /**
     * Get the amount of link integrity errors, that occurred.
     */
    uint64_t GetLocalLinkIntegrityErrors() const {
        return m_localLinkIntegrityErrors;
    }

    /**
     * Get the amount of buffer overrun errors, that occurred.
     */
    uint64_t GetExcessiveBufferOverrunErrors() const {
        return m_excessiveBufferOverrunErrors;
    }

    /**
     * Get the amount of dropped packets on virtual lane 15.
     */
    uint64_t GetVL15Dropped() const {
        return m_vl15Dropped;
    }

    /**
     * Get the transmission-wait counter.
     */
    uint64_t GetXmitWait() const {
        return m_xmitWait;
    }

    /**
     * Write all counters to an output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const IbPerfCounter &o) {
        return os
                << "XmitData: " << o.m_xmitDataBytes << " Bytes" << std::endl
                << "RcvData: " << o.m_rcvDataBytes << " Bytes" << std::endl
                << "XmitPkts: " << o.m_xmitPkts << std::endl
                << "RcvPkts: " << o.m_rcvPkts << std::endl << std::endl
                << "UnicastXmitPkts: " << o.m_unicastXmitPkts << std::endl
                << "UnicastRcvPkts: " << o.m_unicastRcvPkts << std::endl
                << "MulitcastXmitPkts: " << o.m_multicastXmitPkts << std::endl
                << "MulitcastRcvPkts: " << o.m_multicastRcvPkts << std::endl << std::endl
                << "SymbolErrors: " << o.m_symbolErrors << std::endl
                << "LinkDownedCounter: " << o.m_linkDowned << std::endl
                << "LinkRecoveryCounter: " << o.m_linkDowned << std::endl
                << "RcvErrors: " << o.m_rcvErrors << std::endl
                << "RcvRemotePhysicalErrors: " << o.m_rcvRemotePhysicalErrors << std::endl
                << "RcvSwitchRelayErrors: " << o.m_rcvSwitchRelayErrors << std::endl
                << "XmitDiscards: " << o.m_xmitDiscards << std::endl
                << "XmitConstraintErrors: " << o.m_xmitConstraintErrors << std::endl
                << "RcvConstraintErrors: " << o.m_rcvConstraintErrors << std::endl
                << "LocalLinkIntegrityErrors: " << o.m_localLinkIntegrityErrors << std::endl
                << "ExcessiveBufferOverrunErrors: " << o.m_excessiveBufferOverrunErrors << std::endl
                << "VL15Dropped: " << o.m_vl15Dropped << std::endl
                << "XmitWait: " << o.m_xmitWait;
    }

protected:
    /* Counter variables */

    uint64_t m_xmitDataBytes;
    uint64_t m_rcvDataBytes;
    uint64_t m_xmitPkts;
    uint64_t m_rcvPkts;

    uint64_t m_unicastXmitPkts;
    uint64_t m_unicastRcvPkts;
    uint64_t m_multicastXmitPkts;
    uint64_t m_multicastRcvPkts;

    uint64_t m_symbolErrors;
    uint64_t m_linkDowned;
    uint64_t m_linkRecoveries;
    uint64_t m_rcvErrors;
    uint64_t m_rcvRemotePhysicalErrors;
    uint64_t m_rcvSwitchRelayErrors;
    uint64_t m_xmitDiscards;
    uint64_t m_xmitConstraintErrors;
    uint64_t m_rcvConstraintErrors;
    uint64_t m_localLinkIntegrityErrors;
    uint64_t m_excessiveBufferOverrunErrors;
    uint64_t m_vl15Dropped;
    uint64_t m_xmitWait;

protected:
    /**
     * Reset all counter variables to 0.
     */
    void ResetVariables();
};

}

#endif
