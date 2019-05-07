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

#ifndef IBPERFLIB_IBDIAGPERFCOUNTER_H
#define IBPERFLIB_IBDIAGPERFCOUNTER_H

#include <cstdint>
#include <fstream>

namespace IbPerfLib {

/*
 * Reads the diagnostic counters of a local device from
 * "/sys/class/infiniband/<device name>/ports/<port number>/hw_counters/".
 */
class IbDiagPerfCounter {

public:
    /**
     * Constructor.
     *
     * @param deviceName Name of the device, whose diagnostic counters shall be monitored
     * @param portNumber The port, whose diagnostic counters shall be monitored (Set to 0 to monitor the whole device)
     */
    explicit IbDiagPerfCounter(std::string deviceName, uint8_t portNumber);

    /**
     * Destructor.
     */
    ~IbDiagPerfCounter();

    /**
     * Reset all diagnostic counters.
     */
    void ResetCounters();

    /**
     * Query all diagnostic counters.
     * The resulting values will be saved in the counter variables.
     */
    void RefreshCounters();

    /**
     * Get the name of the device, that this counter belongs to.
     */
    std::string GetDeviceName() {
        return m_deviceName;
    }

    /**
     * Get the number of the port, that this counter is monitoring.
     * A port number of 0 means, that the whole device is being monitored.
     */
    uint8_t GetPortNumber() {
        return m_portNumber;
    }

    /**
     * Get the lifespan counter.
     */
    uint64_t GetLifespan() const {
        return m_lifespan;
    }

    /**
     * Get the counter for local length errors as responder.
     */
    uint64_t GetRqLocalLengthErrors() const {
        return m_rqLocalLengthErrors;
    }

    /**
     * Get the counter for local protection errors as responder.
     */
    uint64_t GetRqLocalProtectionErrors() const {
        return m_rqLocalProtectionErrors;
    }

    /**
     * Get the counter for local queue pair protection errors as responder.
     */
    uint64_t GetRqLocalQpProtectionErrors() const {
        return m_rqLocalQpProtectionErrors;
    }

    /**
     * Get the counter for out of sequence errors as responder.
     */
    uint64_t GetRqOutOfSequenceErrors() const {
        return m_rqOutOfSequenceErrors;
    }

    /**
     * Get the counter for remote access errors as responder.
     */
    uint64_t GetRqRemoteAccessErrors() const {
        return m_rqRemoteAccessErrors;
    }

    /**
     * Get the counter for remote invalid request errors as responder.
     */
    uint64_t GetRqRemoteInvalidRequestErrors() const {
        return m_rqRemoteInvalidRequestErrors;
    }

    /**
     * Get the counter for RNR NAKs as responder.
     */
    uint64_t GetRqRnrNakNum() const {
        return m_rqRnrNakNum;
    }

    /**
     * Get the counter for completion queue entry errors as responder.
     */
    uint64_t GetRqCompletionQueueEntryErrors() const {
        return m_rqCompletionQueueEntryErrors;
    }

    /**
     * Get the counter for bad response errors as requester.
     */
    uint64_t GetSqBadResponseErrors() const {
        return m_sqBadResponseErrors;
    }

    /**
     * Get the counter for local length errors as requester.
     */
    uint64_t GetSqLocalLengthErrors() const {
        return m_sqLocalLengthErrors;
    }

    /**
     * Get the counter for local protection errors as requester.
     */
    uint64_t GetSqLocalProtectionErrors() const {
        return m_sqLocalProtectionErrors;
    }

    /**
     * Get the counter for local queue pair protections errors as requester.
     */
    uint64_t GetSqLocalQpProtectionErrors() const {
        return m_sqLocalQpProtectionErrors;
    }

    /**
     * Get the counter for memory window bind errors as requester.
     */
    uint64_t GetSqMemoryWindowBindErrors() const {
        return m_sqMemoryWindowBindErrors;
    }

    /**
     * Get the counter for out of sequence errors as requester.
     */
    uint64_t GetSqOutOfSequenceErrors() const {
        return m_sqOutOfSequenceErrors;
    }

    /**
     * Get the counter for remote access errors as requester.
     */
    uint64_t GetSqRemoteAccessErrors() const {
        return m_sqRemoteAccessErrors;
    }

    /**
     * Get the counter for remote invalid request errors as requester.
     */
    uint64_t GetSqRemoteInvalidRequestErrors() const {
        return m_sqRemoteInvalidRequestErrors;
    }

    /**
     * Get the counter for RNR NAKs as requester.
     */
    uint64_t GetSqRnrNakNum() const {
        return m_sqRnrNakNum;
    }

    /**
     * Get the counter for remote operation errors as requester.
     */
    uint64_t GetSqRemoteOperationErrors() const {
        return m_sqRemoteOperationErrors;
    }

    /**
     * Get the counter for RNR NAK retries exceeded errors as requester.
     */
    uint64_t GetSqRnrNakRetriesExceededErrors() const {
        return m_sqRnrNakRetriesExceededErrors;
    }

    /**
     * Get the counter for transport retries exceeeded errors as requester.
     */
    uint64_t GetSqTransportRetriesExceededErrors() const {
        return m_sqTransportRetriesExceededErrors;
    }

    /**
     * Get the counter for completion queue entry errors as requester.
     */
    uint64_t GetSqCompletionQueueEntryErrors() const {
        return m_sqCompletionQueueEntryErrors;
    }

    /**
     * Write all counters to an output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const IbDiagPerfCounter &o) {
        return os
                << "Device: " << o.m_deviceName
                << (o.m_portNumber > 0 ? ", Port: "  + std::to_string(o.m_portNumber) : "") << std::endl
                << "    Lifespan: " << o.m_lifespan << std::endl
                << "    RqLocalLengthErrors: " << o.m_rqLocalLengthErrors << std::endl
                << "    RqLocalProtectionErrors: " << o.m_rqLocalProtectionErrors << std::endl
                << "    RqLocalQpProtectionErrors: " << o.m_rqLocalQpProtectionErrors << std::endl
                << "    RqOutOfSequenceErrors: " << o.m_rqOutOfSequenceErrors << std::endl
                << "    RqRemoteAccessErrors: " << o.m_rqRemoteAccessErrors << std::endl
                << "    RqRemoteInvalidRequestErrors: " << o.m_rqRemoteInvalidRequestErrors << std::endl
                << "    RqRnrNakNum: " << o.m_rqRnrNakNum << std::endl
                << "    RqCompletionQueueEntryErrors: " << o.m_rqCompletionQueueEntryErrors << std::endl
                << "    SqBadResponseErrors: " << o.m_sqBadResponseErrors << std::endl
                << "    SqLocalLengthErrors: " << o.m_sqLocalLengthErrors << std::endl
                << "    SqLocalProtectionErrors: " << o.m_sqLocalProtectionErrors << std::endl
                << "    SqLocalQpProtectionErrors: " << o.m_sqLocalQpProtectionErrors << std::endl
                << "    SqMemoryWindowBindErrors: " << o.m_sqMemoryWindowBindErrors << std::endl
                << "    SqOutOfSequenceErrors: " << o.m_sqOutOfSequenceErrors << std::endl
                << "    SqRemoteAccessErrors: " << o.m_sqRemoteAccessErrors << std::endl
                << "    SqRemoteInvalidRequestErrors: " << o.m_sqRemoteInvalidRequestErrors << std::endl
                << "    SqRnrNakNum: " << o.m_sqRnrNakNum << std::endl
                << "    SqRemoteOperationErrors: " << o.m_sqRemoteOperationErrors << std::endl
                << "    SqRnrNakRetriesExceededErrors: " << o.m_sqRnrNakRetriesExceededErrors << std::endl
                << "    SqTransportRetriesExceededErrors: " << o.m_sqTransportRetriesExceededErrors << std::endl
                << "    SqCompletionQueueEntryErrors: " << o.m_sqCompletionQueueEntryErrors;
    }


private:

    /**
     * Read a single diagnostic counter.
     *
     * @param index Index into files
     */
    uint64_t ReadCounter(uint8_t index);

private:
    /* Diagnostic counter variables - Only available on local devices */

    uint64_t m_lifespan;

    uint64_t m_rqLocalLengthErrors;
    uint64_t m_rqLocalProtectionErrors;
    uint64_t m_rqLocalQpProtectionErrors;
    uint64_t m_rqOutOfSequenceErrors;
    uint64_t m_rqRemoteAccessErrors;
    uint64_t m_rqRemoteInvalidRequestErrors;
    uint64_t m_rqRnrNakNum;
    uint64_t m_rqCompletionQueueEntryErrors;

    uint64_t m_sqBadResponseErrors;
    uint64_t m_sqLocalLengthErrors;
    uint64_t m_sqLocalProtectionErrors;
    uint64_t m_sqLocalQpProtectionErrors;
    uint64_t m_sqMemoryWindowBindErrors;
    uint64_t m_sqOutOfSequenceErrors;
    uint64_t m_sqRemoteAccessErrors;
    uint64_t m_sqRemoteInvalidRequestErrors;
    uint64_t m_sqRnrNakNum;
    uint64_t m_sqRemoteOperationErrors;
    uint64_t m_sqRnrNakRetriesExceededErrors;
    uint64_t m_sqTransportRetriesExceededErrors;
    uint64_t m_sqCompletionQueueEntryErrors;

private:

    std::string m_deviceName;
    uint8_t m_portNumber;

    char buffer[128];

    std::ifstream files[22];
    uint64_t baseValues[22];
};

}

#endif
