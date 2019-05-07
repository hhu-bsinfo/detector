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

#ifndef IBPERFLIB_IBFABRIC_H
#define IBPERFLIB_IBFABRIC_H

#include "IbNode.h"

namespace IbPerfLib {

/**
 * Represents an entire InfiniBand-fabric.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de, 05.05.2018
 */
class IbFabric {

public:
    /**
     * Constructor.
     *
     * Set to compatibility to true, to activate compatibility mode.
     * In this mode, IbPerfLib will use the filesystem to query performance counters instead of the ibmad-library.
     * In contrary to the ibmad-library, this mode does not require root privileges,
     * but it will only detect local InfiniBand devices.
     *
     * @param compatibility Set to true, to activate compatibility mode.
     */
    explicit IbFabric(bool compatibility = true);

    /**
     * Destructor.
     */
    ~IbFabric();

    /**
     * Refreshes the performance counters on all nodes in the fabric.
     */
    void RefreshCounters();

    /**
     * Resets the performance counters on all nodes in the fabric.
     */
    void ResetCounters();

    /**
     * Get the amount of nodes in the fabric.
     */
    uint32_t GetNumNodes() const {
        return m_nodes.size();
    }

    /**
     * Get all of the nodes in the fabric in a vector.
     */
    std::vector<IbNode *> &GetNodes() {
        return m_nodes;
    }

    /**
     * Write fabric information to an output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const IbFabric &o) {
        os << "Discovered " << o.m_nodes.size() << (o.m_nodes.size() == 1 ? " node" : " nodes")
            << " in the fabric:" << std::endl;

        for (const auto &node : o.m_nodes) {
            os << *node << std::endl;
        }

        return os;
    }

private:

    void discoverFabric();

    void discoverLocalDevices();

private:
    /**
     * Pointer to an ibnd_fabric-struct.
     * The ibnetdisc-library can be used to fill it with information about the fabric.
     */
    ibnd_fabric_t *m_fabric;

    /**
     * All of the nodes in the fabric.
     */
    std::vector<IbNode *> m_nodes;
};

}

#endif