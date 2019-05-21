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

#ifndef IBPERFLIB_IBNODE_H
#define IBPERFLIB_IBNODE_H

#include <cstdint>
#include <ibnetdisc.h>
#include <verbs.h>
#include <vector>
#include "IbPort.h"

namespace IbPerfLib {

/**
 * Represents a node in an InfiniBand-fabric (e.g. a Switch or an HCA).
 * The performance counters are aggregated over all of the node's ports.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class IbNode : public IbPerfCounter {

public:
    /**
     * Constructor.
     *
     * @param node Pointer to an ibnd_node-struct, that has been initialized by the ibnetdisc-library.
     */
    explicit IbNode(ibnd_node_t *node);

    /**
     * Compatibility constructor.
     *
     * Initializes an instance of IbNode with information from an ibverbs-context instead of an ibnd_node-struct.
     * Uses IbPortCompat instead of IbPort, when compat is set to true.
     *
     * @param device The device context to use for this node.
     * @param compatibility Whether to use IbPortCompat or IbPort
     */
    explicit IbNode(ibv_device *device, bool compatibility);

    /**
     * Destructor.
     */
    ~IbNode() override;

    /**
     * Overriding function from IbPerfCounter.
     *
     * CAUTION: This resets the counters on all of the node's ports!
     *          If you want to reset only a single port, use IbNode::GetPorts to get the ports by themselves.
     */
    void ResetCounters() override;

    /**
     * Overriding function from IbPerfCounter.
     *
     * CAUTION: This refreshes the counters on all of the node's ports!
     *          If you want to query only a single port, use IbNode::GetPorts to get the ports by themselves.
     */
    void RefreshCounters() override;

    /**
     * Get the node's description;
     */
    std::string GetDescription() {
        return m_desc;
    }

    /**
     * Get the node's global unique id;
     */
    uint64_t GetGuid() const {
        return m_guid;
    }

    /**
     * Get the amount of ports the node has.
     */
    uint8_t GetNumPorts() const {
        return m_numPorts;
    }

    /**
     * Get all of the node's ports in a vector.
     */
    std::vector<IbPort *> &GetPorts() {
        return m_ports;
    }

    /**
     * Write node information to an output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const IbNode &o) {
        os << "Name: " << o.m_desc << ", "
           << "GUID: 0x" << std::hex << o.m_guid << ", "
           << "Ports: " << std::dec << unsigned(o.m_numPorts) << std::endl;

        for (uint32_t i = 0; i < o.m_ports.size(); i++) {
            os << "    " << *(o.m_ports[i]);

            if(i < o.m_ports.size() - 1) {
                os << std::endl;
            }
        }

        return os;
    }

private:
    /**
     * A short string describing the node (e.g. hostname, manufacturer, ...)
     */
    std::string m_desc;

    /**
     * The node's global unique id.
     */
    uint64_t m_guid;

    /**
     * The amount of ports the node has.
     */
    uint8_t m_numPorts;

    /**
     * All of the node's ports.
     */
    std::vector<IbPort *> m_ports;
};

}

#endif
