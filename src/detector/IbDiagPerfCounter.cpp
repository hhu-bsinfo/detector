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
#include "detector/exception/IbFileException.h"
#include "IbDiagPerfCounter.h"

namespace Detector {

IbDiagPerfCounter::IbDiagPerfCounter(std::string deviceName, uint8_t portNumber) :
        m_lifespan(0),
        m_rqLocalLengthErrors(0),
        m_rqLocalProtectionErrors(0),
        m_rqLocalQpProtectionErrors(0),
        m_rqOutOfSequenceErrors(0),
        m_rqRemoteAccessErrors(0),
        m_rqRemoteInvalidRequestErrors(0),
        m_rqRnrNakNum(0),
        m_rqCompletionQueueEntryErrors(0),
        m_sqBadResponseErrors(0),
        m_sqLocalLengthErrors(0),
        m_sqLocalProtectionErrors(0),
        m_sqLocalQpProtectionErrors(0),
        m_sqMemoryWindowBindErrors(0),
        m_sqOutOfSequenceErrors(0),
        m_sqRemoteAccessErrors(0),
        m_sqRemoteInvalidRequestErrors(0),
        m_sqRnrNakNum(0),
        m_sqRemoteOperationErrors(0),
        m_sqRnrNakRetriesExceededErrors(0),
        m_sqTransportRetriesExceededErrors(0),
        m_sqCompletionQueueEntryErrors(0),
        m_deviceName(std::move(deviceName)),
        m_portNumber(portNumber),
        m_buffer(),
        m_files(),
        m_baseValues() {
    std::string path = "/sys/class/infiniband/" + m_deviceName + (m_portNumber > 0 ?
                       "/ports/" + std::to_string(m_portNumber) + "/hw_counters/" : "/hw_counters/");

    m_files[0] = std::ifstream(path + "lifespan", std::ios::in);
    m_files[1] = std::ifstream(path + "rq_num_lle", std::ios::in);
    m_files[2] = std::ifstream(path + "rq_num_lpe", std::ios::in);
    m_files[3] = std::ifstream(path + "rq_num_lqpoe", std::ios::in);
    m_files[4] = std::ifstream(path + "rq_num_oos", std::ios::in);
    m_files[5] = std::ifstream(path + "rq_num_rae", std::ios::in);
    m_files[6] = std::ifstream(path + "rq_num_rire", std::ios::in);
    m_files[7] = std::ifstream(path + "rq_num_rnr", std::ios::in);
    m_files[8] = std::ifstream(path + "rq_num_wrfe", std::ios::in);
    m_files[9] = std::ifstream(path + "sq_num_bre", std::ios::in);
    m_files[10] = std::ifstream(path + "sq_num_lle", std::ios::in);
    m_files[11] = std::ifstream(path + "sq_num_lpe", std::ios::in);
    m_files[12] = std::ifstream(path + "sq_num_lqpoe", std::ios::in);
    m_files[13] = std::ifstream(path + "sq_num_mwbe", std::ios::in);
    m_files[14] = std::ifstream(path + "sq_num_oos", std::ios::in);
    m_files[15] = std::ifstream(path + "sq_num_rae", std::ios::in);
    m_files[16] = std::ifstream(path + "sq_num_rire", std::ios::in);
    m_files[17] = std::ifstream(path + "sq_num_rnr", std::ios::in);
    m_files[18] = std::ifstream(path + "sq_num_roe", std::ios::in);
    m_files[19] = std::ifstream(path + "sq_num_rree", std::ios::in);
    m_files[20] = std::ifstream(path + "sq_num_tree", std::ios::in);
    m_files[21] = std::ifstream(path + "sq_num_wrfe", std::ios::in);

    for (const std::ifstream &file : m_files) {
        if (!file.is_open()) {
            throw IbFileException("Unable to open file!");
        }
    }

    std::memset(m_baseValues, 0, sizeof(m_baseValues));
}

IbDiagPerfCounter::~IbDiagPerfCounter() {
    for (std::ifstream &file : m_files) {
        file.close();
    }
}

void IbDiagPerfCounter::ResetCounters() {
    for(uint32_t i = 0; i < sizeof(m_files) / sizeof(std::ifstream); i++) {
        m_files[i].seekg(0, std::ios::beg);
        m_files[i].getline(m_buffer, sizeof(m_buffer));

        m_baseValues[i] = strtoull(m_buffer, nullptr, 10);
    }
}

void IbDiagPerfCounter::RefreshCounters() {
    m_lifespan = ReadCounter(0);
    m_rqLocalLengthErrors = ReadCounter(1);
    m_rqLocalProtectionErrors = ReadCounter(2);
    m_rqLocalQpProtectionErrors = ReadCounter(3);
    m_rqOutOfSequenceErrors = ReadCounter(4);
    m_rqRemoteAccessErrors = ReadCounter(5);
    m_rqRemoteInvalidRequestErrors = ReadCounter(6);
    m_rqRnrNakNum = ReadCounter(7);
    m_rqCompletionQueueEntryErrors = ReadCounter(8);
    m_sqBadResponseErrors = ReadCounter(9);
    m_sqLocalLengthErrors = ReadCounter(10);
    m_sqLocalProtectionErrors = ReadCounter(11);
    m_sqLocalQpProtectionErrors = ReadCounter(12);
    m_sqMemoryWindowBindErrors = ReadCounter(13);
    m_sqOutOfSequenceErrors = ReadCounter(14);
    m_sqRemoteAccessErrors = ReadCounter(15);
    m_sqRemoteInvalidRequestErrors = ReadCounter(16);
    m_sqRnrNakNum = ReadCounter(17);
    m_sqRemoteOperationErrors = ReadCounter(18);
    m_sqRnrNakRetriesExceededErrors = ReadCounter(19);
    m_sqTransportRetriesExceededErrors = ReadCounter(20);
    m_sqCompletionQueueEntryErrors = ReadCounter(21);
}

uint64_t IbDiagPerfCounter::ReadCounter(uint8_t index) {
    std::memset(m_buffer, 0, sizeof(m_buffer));

    if(!m_files[index].good()) {
        throw IbFileException("Unable to read file!");
    }

    m_files[index].seekg(0, std::ios::beg);
    m_files[index].getline(m_buffer, sizeof(m_buffer));

    return strtoull(m_buffer, nullptr, 10) - m_baseValues[index];
}

}
