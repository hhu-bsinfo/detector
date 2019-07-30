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

#include <algorithm>
#include "IbFabric.h"
#include "detector/exception/IbMadException.h"
#include "detector/exception/IbNetDiscException.h"
#include "detector/exception/IbFileException.h"
#include "detector/exception/IbVerbsException.h"
#include "IbDiagPerfCounter.h"

namespace Detector {

IbFabric::IbFabric(bool network, bool compatibility) :
        m_fabric(nullptr) {

    discoverFabric(network, compatibility);

    std::sort(m_nodes.begin(), m_nodes.end(),
              [](IbNode *l, IbNode *r) { return l->GetDescription() < r->GetDescription(); });
}

IbFabric::~IbFabric() {
    for (IbNode *node : m_nodes) {
        delete node;
    }

    if(m_fabric != nullptr) {
        ibnd_destroy_fabric(m_fabric);
    }
}

void IbFabric::RefreshCounters() {
    for (IbNode *node : m_nodes) {
        node->RefreshCounters();
    }
}

void IbFabric::ResetCounters() {
    for (IbNode *node : m_nodes) {
        node->ResetCounters();
    }
}

void IbFabric::discoverFabric(bool network, bool compatibility) {
    if(compatibility) {
        discoverLocalDevices(true);
    } else {
        if(network) {
            discoverNetwork();
        } else {
            discoverLocalDevices(false);
        }
    }
}

void IbFabric::discoverNetwork() {
    // The config contains parameters for ibnd_discover_fabric.
    // We don't need any special parameters and just set them all to zero.
    ibnd_config_t config = {0};

    // ibnd_discover_fabric() scans the entire InfiniBand-fabric for nodes.
    // It takes the following parameters:
    //
    // dev_name: The name of the local device, from which the discovery is started.
    //           This seems to be optional, as passing a nullptr also works.
    // dev_port: This seems to be number of the local port from which the discovery is started.
    //           Passing a zero works fine. I guess, it then uses a default value.
    // from:     This seems to be a portid-struct, that describes the local port, from which the discovery is started.
    //           Again, passing a nullptr works fine.
    // config:   Contains some configuration parameters for ibnd_discover_fabric()
    m_fabric = ibnd_discover_fabric(nullptr, 0, nullptr, &config);

    if (m_fabric == nullptr) {
        throw IbNetDiscException("Unable to discover nodes in the fabric (ibnd_discover_fabric failed)!");
    }

    // The found nodes are stored in a linked list, where every node has a pointer to the next one.
    ibnd_node_t *currentNode = &m_fabric->nodes[0];

    // Iterate over all nodes and create an instance of IbPort for each one.
    do {
        m_nodes.emplace_back(new IbNode(currentNode));
        currentNode = currentNode->next;
    } while (currentNode != nullptr);
}

void IbFabric::discoverLocalDevices(bool compatibility) {
    int32_t numDevices;
    ibv_device **deviceList = ibv_get_device_list(&numDevices);

    if(deviceList == nullptr) {
        throw IbVerbsException("Unable to get device list! Error: " + std::string(strerror(errno)));
    }

    for(int32_t i = 0; i < numDevices; i++) {
        IbNode *currentNode = nullptr;

        try {
            currentNode = new IbNode(deviceList[i], compatibility);
        } catch(const IbVerbsException &exception) {
            continue;
        }

        m_nodes.emplace_back(currentNode);
    }

    ibv_free_device_list(deviceList);
}

}
