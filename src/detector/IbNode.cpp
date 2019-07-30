#include <detector/exception/IbVerbsException.h>

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

#include "IbNode.h"
#include "IbPortCompat.h"

namespace Detector {

IbNode::IbNode(ibv_device *device, bool compat) : IbPerfCounter(),
                                                   m_guid(0),
                                                   m_numPorts(0) {
    m_desc = ibv_get_device_name(device);

    ibv_context *context = ibv_open_device(device);

    if (device == nullptr) {
        throw IbVerbsException("Unable to open context for device '" + m_desc + "'! Error: "+ strerror(errno));
    }

    ibv_device_attr attr{};

    int ret = ibv_query_device(context, &attr);

    if(ret != 0) {
        throw IbVerbsException("Unable to query device attributes from '" + m_desc + "'! Error: " + strerror(ret));
    }

    m_guid = htonll(attr.node_guid);
    m_numPorts = attr.phys_port_cnt;

    // Iterate over all of the node's ports and create an instance of IbPortCompat for each one.
    for (uint8_t i = 0; i < m_numPorts; i++) {
        ibv_port_attr portAttributes{};
        ret = ibv_query_port(context, static_cast<uint8_t>(i + 1), &portAttributes);

        if(ret != 0) {
            throw IbVerbsException("Unable to query port attributes of '" + m_desc +
                                    + ", port " + std::to_string(i) + "'! Error: " + std::string(strerror(ret)));
        }

        IbPort *port;

        if(compat) {
            port = new IbPortCompat(m_desc, portAttributes, static_cast<uint8_t>(i + 1));
        } else {
            port = new IbPort(portAttributes.lid, static_cast<uint8_t>(i + 1));
        }

        m_ports.push_back(port);
    }

    ibv_close_device(context);
}

IbNode::IbNode(ibnd_node_t *node) : IbPerfCounter(),
                                    m_desc(node->nodedesc),
                                    m_guid(node->guid),
                                    m_numPorts(static_cast<uint8_t>(node->numports)) {
    // Iterate over all of the node's ports and create an instance of IbPort for each one.
    for (uint8_t i = 0; i < m_numPorts; i++) {
        ibnd_port *currentPort = node->ports[i + 1];

        if (currentPort != nullptr) {
            m_ports.push_back(new IbPort(currentPort->base_lid, static_cast<uint8_t>(currentPort->portnum)));
        }
    }
}

IbNode::~IbNode() {
    for (IbPort *port : m_ports) {
        delete port;
    }
}

void IbNode::ResetCounters() {
    ResetVariables();

    for (IbPort *port : m_ports) {
        port->ResetCounters();
    }
}

void IbNode::RefreshCounters() {
    ResetVariables();

    for (IbPort *port : m_ports) {
        port->RefreshCounters();

        m_xmitDataBytes += port->GetXmitDataBytes();
        m_rcvDataBytes += port->GetRcvDataBytes();
        m_xmitPkts += port->GetXmitPkts();
        m_rcvPkts += port->GetRcvPkts();

        m_unicastXmitPkts += port->GetUnicastXmitPkts();
        m_unicastRcvPkts += port->GetUnicastRcvPkts();
        m_multicastXmitPkts += port->GetMulticastXmitPkts();
        m_multicastRcvPkts += port->GetMulticastRcvPkts();

        m_symbolErrors += port->GetSymbolErrors();
        m_linkDowned += port->GetLinkDownedCounter();
        m_linkRecoveries += port->GetLinkRecoveryCounter();
        m_rcvErrors += port->GetRcvErrors();
        m_rcvRemotePhysicalErrors += port->GetRcvRemotePhysicalErrors();
        m_rcvSwitchRelayErrors += port->GetRcvSwitchRelayErrors();
        m_xmitDiscards += port->GetXmitDiscards();
        m_xmitConstraintErrors += port->GetXmitConstraintErrors();
        m_rcvConstraintErrors += port->GetRcvConstraintErrors();
        m_localLinkIntegrityErrors += port->GetLocalLinkIntegrityErrors();
        m_excessiveBufferOverrunErrors += port->GetExcessiveBufferOverrunErrors();
        m_vl15Dropped += port->GetVL15Dropped();
        m_xmitWait += port->GetXmitWait();
    }
}

}