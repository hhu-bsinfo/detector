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

#include <dirent.h>
#include <cstring>
#include <csignal>
#include <iostream>
#include <chrono>
#include <thread>
#include <IbPerfLib/IbDiagPerfCounter.h>
#include <IbPerfLib/IbFileException.h>
#include <vector>

bool isRunning = true;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char *argv[]) {
    std::vector<IbPerfLib::IbDiagPerfCounter *> counters;

    DIR *infinibandDirectory = opendir("/sys/class/infiniband/");

    if (infinibandDirectory == nullptr) {
        printf("Unable to open '/sys/class/infiniband/'! Error: %s", strerror(errno));
        exit(1);
    }

    dirent *device;

    while ((device = readdir(infinibandDirectory)) != nullptr) {
        if (std::string(device->d_name) == "." || std::string(device->d_name) == "..") {
            continue;
        }

        counters.emplace_back(new IbPerfLib::IbDiagPerfCounter(device->d_name, 0));

        std::string path = std::string("/sys/class/infiniband/") + device->d_name + "/ports/";

        DIR *deviceDirectory = opendir(path.c_str());

        if (deviceDirectory == nullptr) {
            printf("Unable to open '%s'! Error: %s", path.c_str(), strerror(errno));
            exit(1);
        }

        dirent *port;

        while ((port = readdir(deviceDirectory)) != nullptr) {
            if (std::string(port->d_name) == "." || std::string(port->d_name) == "..") {
                continue;
            }

            auto portNumber = static_cast<uint8_t>(strtoul(port->d_name, nullptr, 10));

            counters.emplace_back(new IbPerfLib::IbDiagPerfCounter(device->d_name, portNumber));
        }
    }

    signal(SIGINT, SignalHandler);

    while (isRunning) {
        try {
            for(const auto &diagCounter : counters) {
                diagCounter->RefreshCounters();
                std::cout << *diagCounter << std::endl << std::endl;
            }
        } catch (const IbPerfLib::IbFileException &exception) {
            printf("An exception occurred: %s", exception.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    for(const auto &diagCounter : counters) {
        delete diagCounter;
    }
}