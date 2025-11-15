#pragma once
#include <exception>
#include <string>

enum class ErrorLevel {Information, Warning, Critical};
namespace sxEditCore{
    class sxException: public std::exception{
        private:
            const std::string message;
            const std::string separator = " | ";
            ErrorLevel level;
            std::string errorCode;
        public:
            /*
                Throws an exception with a given error message and error level.
                Valid error levels are:
                - Information
                - Warning
                - Critical
                Diffrent error levels will result in diffrent error window styles.
            */
            explicit sxException(const std::string errorMessage, const ErrorLevel errorLevel, std::string errorCode = "D00"): message(errorMessage), level(errorLevel), errorCode(errorCode) {}
            ErrorLevel getErrorLevel() const {
                return level;
            }
            const char* what() const noexcept override {
                return (errorCode + separator + message).c_str();
            }
    };

}