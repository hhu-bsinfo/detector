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

#include "IbPerfCounter.h"

namespace Detector {

IbPerfCounter::IbPerfCounter() :
        m_xmitDataBytes(0),
        m_rcvDataBytes(0),
        m_xmitPkts(0),
        m_rcvPkts(0),
        m_unicastXmitPkts(0),
        m_unicastRcvPkts(0),
        m_multicastXmitPkts(0),
        m_multicastRcvPkts(0),
        m_symbolErrors(0),
        m_linkDowned(0),
        m_linkRecoveries(0),
        m_rcvErrors(0),
        m_rcvRemotePhysicalErrors(0),
        m_rcvSwitchRelayErrors(0),
        m_xmitDiscards(0),
        m_xmitConstraintErrors(0),
        m_rcvConstraintErrors(0),
        m_localLinkIntegrityErrors(0),
        m_excessiveBufferOverrunErrors(0),
        m_vl15Dropped(0),
        m_xmitWait(0) {

}

void IbPerfCounter::ResetVariables() {
    m_xmitDataBytes = 0;
    m_rcvDataBytes = 0;
    m_xmitPkts = 0;
    m_rcvPkts = 0;

    m_unicastXmitPkts = 0;
    m_unicastRcvPkts = 0;
    m_multicastXmitPkts = 0;
    m_multicastRcvPkts = 0;

    m_symbolErrors = 0;
    m_linkDowned = 0;
    m_linkRecoveries = 0;
    m_rcvErrors = 0;
    m_rcvRemotePhysicalErrors = 0;
    m_rcvSwitchRelayErrors = 0;
    m_xmitDiscards = 0;
    m_xmitConstraintErrors = 0;
    m_rcvConstraintErrors = 0;
    m_localLinkIntegrityErrors = 0;
    m_excessiveBufferOverrunErrors = 0;
    m_vl15Dropped = 0;
    m_xmitWait = 0;
}

}