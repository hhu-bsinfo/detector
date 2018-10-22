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

#ifndef PROJECT_VERSION_H
#define PROJECT_VERSION_H

#define XSTRINGIFY(a) STRINGIFY(a)
#define STRINGIFY(a) #a


#ifdef IBPERFLIB_VERSION
    static const char* IBPERF_LIB_VERSION = XSTRINGIFY(IBPERFLIB_VERSION);
#else
    static const char* IBPERF_LIB_VERSION = "v0.0.0";
#endif

#ifdef IBPERFLIB_GIT_REV
    static const char* IBPERF_LIB_GIT_REV = XSTRINGIFY(IBPERFLIB_GIT_REV);
#else
    static const char* IBPERF_LIB_GIT_REV = "unknown";
#endif

#ifdef IBPERFLIB_BUILD_DATE
    static const char* IBPERF_LIB_BUILD_DATE = XSTRINGIFY(IBPERFLIB_BUILD_DATE);
#else
    static const char* IBPERF_LIB_BUILD_DATE = "0000-00-00 00:00:00";
#endif


#undef STRINGIFY
#undef XSTRINGIFY

#endif
