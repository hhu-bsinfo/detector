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

#ifndef IBPERFLIB_VERSION_H
#define IBPERFLIB_VERSION_H

namespace IbPerfLib {

/**
 * Holds information about the build cofiguration and program version.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date October 2018
 */
class BuildConfig {

public:

    static const char *VERSION;
    static const char *GIT_REV;
    static const char *GIT_BRANCH;
    static const char *BUILD_DATE;
    static const bool ADDITIONAL_EXTENDED_COUNTERS_ENABLED;

};

}

#endif
