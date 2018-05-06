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

namespace IbPerfLib {

IbNode::IbNode(ibnd_node_t *node) : IbPerfCounter(),
                                    m_guid(node->guid),
                                    m_numPorts(static_cast<uint8_t>(node->numports)) {
    char tmpDesc[IB_SMP_DATA_SIZE];
    memcpy(tmpDesc, node->nodedesc, IB_SMP_DATA_SIZE);

    m_desc = std::string(tmpDesc);

    // Iterate over all of the node's ports an create an instance of IbPort for each one.
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