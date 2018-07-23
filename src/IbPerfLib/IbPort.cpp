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

#include "IbPort.h"
#include "IbMadException.h"

namespace IbPerfLib {

IbPort::IbPort(uint16_t lid, uint8_t portNum) : IbPerfCounter(),
                                                m_lid(lid),
                                                m_portNum(portNum),
                                                m_linkWidth(0),
                                                m_madPort(nullptr),
                                                m_portId({0}),
                                                m_isExtendedWidthSupported(false),
                                                m_isAdditionalExtendedPortCountersSupported(false),
                                                m_isXmitWaitSupported(false) {
    uint8_t pmaQueryBuf[QUERY_BUF_SIZE];
    uint8_t smpQueryBuf[IB_SMP_DATA_SIZE];
    int mgmt_classes[3] = {IB_SMI_CLASS, IB_SA_CLASS, IB_PERFORMANCE_CLASS};
    uint16_t capabilityMask;
    uint32_t capabilityMask2;

    memset(pmaQueryBuf, 0, sizeof(pmaQueryBuf));
    memset(smpQueryBuf, 0, sizeof(smpQueryBuf));

    // Open a MAD-port. mad_rpc_open_port takes the following parameters:
    //
    // dev_name: The name of the local device from which all queries will be sent.
    //           This seems to be optional, as passing a nullptr also works.
    // dev_port: This seems to be number of the local port from which the all queries will be sent.
    //           Passing a zero works fine. I guess, it then uses a default value.
    // mgmt_classes: I guess, this array is used to declare the fields, that we want to access.
    // num_classes: The amount of management-classes.
    m_madPort = mad_rpc_open_port(nullptr, 0, mgmt_classes, 3);

    if (m_madPort == nullptr) {
        throw IbMadException("MAD: Failed to open port! (mad_rpc_open_port failed)");
    }

    // Once the MAD-port has been opened, we can use ib_portid_set to initialize m_portId.
    // It takes the following parameters:
    //
    // portid: A pointer to the ib_portid_t-struct, that shall be initialized.
    // lid: The device's local id.
    // qp: I guess, one can set this value to query only one specific queue pair. Setting it to 0 works fine for me.
    // qkey: Again, setting this to 0 works flawlessy.
    ib_portid_set(&m_portId, m_lid, 0, 0);

    // Query the Performance Management Agent for meta-information. We do this to get the device's capability masks.
    // pma_query_via() takes the following parameters:
    //
    // rcvbuf: The queried data will be written to this buffer. I could not find any information about how big this
    //         buffer has to be. The perfquery-tool uses 1536 Bytes, so I do the same.
    // dest: A pointer to the ib_portid_t-struct, that we initialized earlier.
    // port: The number of the port that shall be queried. In this case 0 works fine.
    // timeout: Setting the timeout to 0 works fine.
    // id: The type of information we want to query.
    // srcport: The MAD-port.
    if (!pma_query_via(pmaQueryBuf, &m_portId, 0, DEFAULT_QUERY_TIMEOUT, CLASS_PORT_INFO, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("MAD: Failed to query port information! (pma_query_via failed)");
    }

    // Get the capability masks from the buffer.
    memcpy(&capabilityMask, pmaQueryBuf + 2, sizeof(capabilityMask));
    memcpy(&capabilityMask2, pmaQueryBuf + 4, sizeof(capabilityMask2));
    capabilityMask2 = htonl(ntohl(capabilityMask2) >> 5);

    if (capabilityMask & IB_PM_EXT_WIDTH_SUPPORTED) {
        m_isExtendedWidthSupported = true;
    }

    if (capabilityMask & IB_PM_PC_XMIT_WAIT_SUP) {
        m_isXmitWaitSupported = true;
    }

    if (capabilityMask2 & IB_PM_IS_ADDL_PORT_CTRS_EXT_SUP) {
        m_isAdditionalExtendedPortCountersSupported = true;
    }

    // Query the Subnet Management Agent for device-information. We do this to get the port's link width.
    // This function works similar to pma_query_via() (see above).
    if (!smp_query_via(smpQueryBuf, &m_portId, IB_ATTR_PORT_INFO, 0, 0, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("MAD: Failed to query device information! (smp_query_via failed)");
    }

    mad_decode_field(smpQueryBuf, IB_PORT_LINK_WIDTH_ACTIVE_F, &m_linkWidth);

    switch (m_linkWidth) {
        case 1:
            m_linkWidth = 1;
            break;
        case 2:
            m_linkWidth = 4;
            break;
        case 4:
            m_linkWidth = 8;
            break;
        case 8:
            m_linkWidth = 12;
            break;
        default:
            m_linkWidth = 1;
            break;
    }
}

IbPort::~IbPort() {
    // Close the MAD-port.
    mad_rpc_close_port(m_madPort);
}

void IbPort::ResetCounters() {
    char resetBuf[RESET_BUF_SIZE];
    memset(resetBuf, 0, sizeof(resetBuf));

    ResetVariables();

    // Resetting the performance counters can be accomplished by calling performance_reset_via().
    // It takes the following parameters:
    //
    // rcvbuf: The function needs a buffer. I don't know what is written to it and what size it should have,
    //         but the perfquery-tool sets it to 1024 Bytes, so I just do the same.
    // dest: The ib_portid-struct.
    // port: The number of the port, whose counters shall be resetted.
    // mask: A bitmask, determining which counters shall be resetted. Setting it to 0xffffffff will reset all counters.
    // timeout: Setting the timeout to 0 works fine.
    // id: The class of counters that shall be resetted. IB_GSI_PORT_COUNTERS are the 32-bit performance counters
    //       and IB_GSI_PORT_COUNTERS_EXT are the 64-bit extended performance counters.
    // srcport: The MAD-port.
    if (!performance_reset_via(resetBuf, &m_portId, m_portNum, 0xffffffff, DEFAULT_QUERY_TIMEOUT,
                               IB_GSI_PORT_COUNTERS, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("Failed to reset performance counters!");
    }

    if (!performance_reset_via(resetBuf, &m_portId, m_portNum, 0xffffffff, DEFAULT_QUERY_TIMEOUT,
                               IB_GSI_PORT_COUNTERS_EXT, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("Failed to reset extended performance counters!");
    }
}

void IbPort::RefreshCounters() {
    uint64_t value64;
    uint32_t value32;
    uint8_t pmaQueryBuf[QUERY_BUF_SIZE];

    ResetVariables();

    // Query the port's performance counters.
    //
    // Reading the performance counters works as follows:
    // 1. Call pma_query_via() to query the counters. Pass either IB_GSI_PORT_COUNTERS or IB_GSI_PORT_COUNTERS_EXT
    //    to read either the normal 32-bit or the extended 64-bit counters.
    // 2. Call mad_decode_field() for every counter we want to get. mad_decode_field() takes the following parameters:
    //
    //    buf: The buffer, that has been filled by pma_query_via().
    //    field: The counter, that we want to read.
    //    val: A pointer to the variable that the counter will be saved in. Make sure it has the correct size.

    // Get the extended 64-bit transmit- and receive-counters.
    memset(pmaQueryBuf, 0, sizeof(pmaQueryBuf));

    if (!pma_query_via(pmaQueryBuf, &m_portId, m_portNum, 0, IB_GSI_PORT_COUNTERS_EXT, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("Failed to query extended performance counters!");
    }

    mad_decode_field(pmaQueryBuf, IB_PC_EXT_XMT_BYTES_F, &value64);
    m_xmitDataBytes += value64 * m_linkWidth;

    mad_decode_field(pmaQueryBuf, IB_PC_EXT_RCV_BYTES_F, &value64);
    m_rcvDataBytes += value64 * m_linkWidth;

    mad_decode_field(pmaQueryBuf, IB_PC_EXT_XMT_PKTS_F, &value64);
    m_xmitPkts += value64;

    mad_decode_field(pmaQueryBuf, IB_PC_EXT_RCV_PKTS_F, &value64);
    m_rcvPkts += value64;

    // Get the extended 64-bit uni- and multicast-counters, if supported by the device.
    if (m_isExtendedWidthSupported) {
        mad_decode_field(pmaQueryBuf, IB_PC_EXT_XMT_UPKTS_F, &value64);
        m_unicastXmitPkts += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_RCV_UPKTS_F, &value64);
        m_unicastRcvPkts += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_XMT_MPKTS_F, &value64);
        m_multicastXmitPkts += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_RCV_MPKTS_F, &value64);
        m_multicastRcvPkts += value64;
    }

#if USE_ADDITIONAL_EXTENDED_COUNTERS
    // Get the extended 64-bit error-counters, if supported by the device.
    if (m_isAdditionalExtendedPortCountersSupported) {
        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_RCV_F, &value64);
        m_rcvErrors += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_PHYSRCV_F, &value64);
        m_rcvRemotePhysicalErrors += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_SWITCH_REL_F, &value64);
        m_rcvSwitchRelayErrors += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_XMT_DISCARDS_F, &value64);
        m_xmitDiscards += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_XMTCONSTR_F, &value64);
        m_xmitConstraintErrors += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_RCVCONSTR_F, &value64);
        m_rcvConstraintErrors += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_LOCALINTEG_F, &value64);
        m_localLinkIntegrityErrors += value64;

        mad_decode_field(pmaQueryBuf, IB_PC_EXT_ERR_EXCESS_OVR_F, &value64);
        m_excessiveBufferOverrunErrors += value64;

        if (m_isXmitWaitSupported) {
            mad_decode_field(pmaQueryBuf, IB_PC_EXT_XMT_WAIT_F, &value64);
            m_xmitWait += value64;
        }
    } else {
#endif
    //Get the normal 32-Bit error-counters, if the device does not support the extended error-counters
    memset(pmaQueryBuf, 0, sizeof(pmaQueryBuf));

    if (!pma_query_via(pmaQueryBuf, &m_portId, m_portNum, 0, IB_GSI_PORT_COUNTERS, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("Failed to query performance counters!");
    }

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_RCV_F, &value32);
    m_rcvErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_PHYSRCV_F, &value32);
    m_rcvRemotePhysicalErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_SWITCH_REL_F, &value32);
    m_rcvSwitchRelayErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_XMT_DISCARDS_F, &value32);
    m_xmitDiscards += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_XMTCONSTR_F, &value32);
    m_xmitConstraintErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_RCVCONSTR_F, &value32);
    m_rcvConstraintErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_LOCALINTEG_F, &value32);
    m_localLinkIntegrityErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_EXCESS_OVR_F, &value32);
    m_excessiveBufferOverrunErrors += value32;

    if (m_isXmitWaitSupported) {
        mad_decode_field(pmaQueryBuf, IB_PC_XMT_WAIT_F, &value32);
        m_xmitWait += value32;
    }
#if USE_ADDITIONAL_EXTENDED_COUNTERS
    }
#endif
    // Get the rest of the counters, that only have 32-bit variants.
    memset(pmaQueryBuf, 0, sizeof(pmaQueryBuf));

    if (!pma_query_via(pmaQueryBuf, &m_portId, m_portNum, 0, IB_GSI_PORT_COUNTERS, m_madPort)) {
        mad_rpc_close_port(m_madPort);
        throw IbMadException("Failed to query performance counters!");
    }

    mad_decode_field(pmaQueryBuf, IB_PC_ERR_SYM_F, &value32);
    m_symbolErrors += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_LINK_DOWNED_F, &value32);
    m_linkDowned += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_LINK_RECOVERS_F, &value32);
    m_linkRecoveries += value32;

    mad_decode_field(pmaQueryBuf, IB_PC_VL15_DROPPED_F, &value32);
    m_vl15Dropped += value32;
}

}