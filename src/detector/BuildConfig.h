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

#ifndef DETECTOR_VERSION_H
#define DETECTOR_VERSION_H

namespace Detector {

/**
 * Holds information about the build configuration and program version.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date October 2018
 */
class BuildConfig {

private:

    static const constexpr char banner[] =
            "    ____       __            __             # Version             : %s\n"
            "   / __ \\___  / /____  _____/ /_____  _____ # Build Date          : %s\n"
            "  / / / / _ \\/ __/ _ \\/ ___/ __/ __ \\/ ___/ # Git Branch          : %s\n"
            " / /_/ /  __/ /_/  __/ /__/ /_/ /_/ / /     # Git Commit          : %s\n"
            "/_____/\\___/\\__/\\___/\\___/\\__/\\____/_/      # Additional counters : %s\n";

public:

    static const char *VERSION;
    static const char *GIT_REV;
    static const char *GIT_BRANCH;
    static const char *BUILD_DATE;
    static const bool ADDITIONAL_EXTENDED_COUNTERS_ENABLED;

public:

    static void printBanner();

};

}

#endif
