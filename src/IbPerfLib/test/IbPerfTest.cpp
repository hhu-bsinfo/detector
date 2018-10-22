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
    printf("IbPerfLib %s - git %s\nBuild date: %s\n\n", IbPerfLib::BuildConfig::VERSION,
            IbPerfLib::BuildConfig::GIT_REV, IbPerfLib::BuildConfig::BUILD_DATE);

    if(argc < 2) {
        printf("Usage: ./IbPerfTest <mad/compat>\n");
        exit(EXIT_FAILURE);
    }

    bool compat;

    if(!strcmp(argv[1], "mad")) {
        compat = false;
    } else if(!strcmp(argv[1], "compat")) {
        compat = true;
    } else {
        printf("Usage: ./IbPerfTest <mad/compat>\n");
        exit(EXIT_FAILURE);
    }

    if(USE_ADDITIONAL_EXTENDED_COUNTERS) {
        printf("Additional extended counters are supported!\n\n");
    } else {
        printf("Additional extended counters are not supported!\n\n");
    }

    IbPerfLib::IbFabric fabric(compat);

    signal(SIGINT, SignalHandler);

    while(isRunning) {
        try {
            fabric.RefreshCounters();
            std::cout << fabric;
        } catch(const IbPerfLib::IbPerfException &exception) {
            printf("An exception occured: %s", exception.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}