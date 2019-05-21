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

#include <csignal>
#include <chrono>
#include <thread>
#include <IbPerfLib/BuildConfig.h>
#include <IbPerfLib/Exception/IbMadException.h>
#include <IbPerfLib/IbFabric.h>

bool isRunning = true;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char *argv[]) {
    printf("IbPerfLib %s - git %s(%s)\nBuild date: %s\nAdditional extended counters: %s\n\n",
            IbPerfLib::BuildConfig::VERSION, IbPerfLib::BuildConfig::GIT_REV, IbPerfLib::BuildConfig::GIT_BRANCH,
            IbPerfLib::BuildConfig::BUILD_DATE, IbPerfLib::BuildConfig::ADDITIONAL_EXTENDED_COUNTERS_ENABLED ?
            "Enabled" : "Disabled");

    if(argc < 3) {
        printf("Usage: ./IbPerfTest <network/local> <mad/compat>\n");
        exit(EXIT_FAILURE);
    }

    bool network;
    bool compat;

    if(!strcmp(argv[1], "network")) {
        network = true;
    } else if(!strcmp(argv[1], "local")) {
        network = false;
    } else {
        printf("Usage: ./IbPerfTest <network/local> <mad/compat>\n");
        exit(EXIT_FAILURE);
    }

    if(!strcmp(argv[2], "mad")) {
        compat = false;
    } else if(!strcmp(argv[2], "compat")) {
        compat = true;
    } else {
        printf("Usage: ./IbPerfTest <network/local> <mad/compat>\n");
        exit(EXIT_FAILURE);
    }

    IbPerfLib::IbFabric fabric(network, compat);

    signal(SIGINT, SignalHandler);

    while(isRunning) {
        try {
            fabric.RefreshCounters();
            std::cout << fabric << std::endl << std::endl;
        } catch(const IbPerfLib::IbPerfException &exception) {
            printf("An exception occurred: %s", exception.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}