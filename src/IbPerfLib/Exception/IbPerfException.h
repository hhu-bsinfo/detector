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

#ifndef IBPERFLIB_IBPERFEXCEPTION_H
#define IBPERFLIB_IBPERFEXCEPTION_H

#include <exception>
#include <string>

namespace IbPerfLib {

/**
 * An exception, which signalises an error in IbPerfLib.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date July 2018
 */
class IbPerfException : public std::exception {

public:

    /**
     * Constructor
     *
     * @param message Error message
     */
    explicit IbPerfException(std::string message) noexcept :
            message(std::move(message)) {

    }

    IbPerfException(const IbPerfException &copy) noexcept {
        this->message = copy.message;
    }

    /**
     * Destructor.
     */
    ~IbPerfException() override = default;

    /**
     * Overriding function from std::exception.
     */
    const char *what() const noexcept override {
        return message.c_str();
    }


private:

    std::string message;

};

}

#endif
