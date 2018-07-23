#ifndef PROJECT_IBMADEXCEPTION_H
#define PROJECT_IBMADEXCEPTION_H

#include <exception>
#include <string>

namespace IbPerfLib {

class IbMadException : public std::exception {

public:

    /**
     * Constructor
     *
     * @param message Error message
     */
    explicit IbMadException(const char *message) :
            message(message) {

    }

    /**
     * Destructor.
     */
    ~IbMadException() override = default;

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
