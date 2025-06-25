#pragma once
#include <string>
#include <windows.h>
#include <exception>
namespace sxEditCore{
    class SXException: public std::exception{
        private:
            std::string message;
        public:
            SXException(const std::string& msg){
                message = msg;
            }
            SXException(const std::string& msg, const HWND windowHandle){
                MessageBoxA(windowHandle,msg.c_str(), "Fatal error occured.", MB_ICONERROR|MB_RETRYCANCEL);
            }
        
            const char* what() const noexcept override{
                return message.c_str();
            }
    };

} 
