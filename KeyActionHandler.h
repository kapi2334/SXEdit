#pragma once

#include <Windows.h>
#include "CursorHandler.h"
#include "Data structures/DoublyLinkedList.h"
#include "WindowUpdateHandler.h"

namespace sxEditCore{
    class KeyActionHandler{
        private:
            UpdateHandler* _windowUpdateHandler = nullptr;

            WCHAR getCharFromWparam(WPARAM key){
                WCHAR tmp[2];
                tmp[0] = (WCHAR)key;
                tmp[1] = L'\0';
                //Checking if SHIFT or CAPS are pressed
                bool shift = (GetKeyState(VK_SHIFT)&0x8000) != 0; //Checking if 15th bit = 1 - it means that shift is pressed.
                bool caps = (GetKeyState(VK_CAPITAL)&0x0001) !=0; //Checking if first bit = 1 - it means that shift is pressed.
                if(!(shift ^ caps)){ //!XOR 
                    tmp[0] = towlower(tmp[0]);
                }
                    
                return *tmp;
            }
        public:
            void registerPress(WPARAM key,dataStructures::dlList& list, HWND hwnd){
                try{
                switch(key){
                    //case 0x09: //TAB
                    case 0x0D: //ENTER
                        _windowUpdateHandler ->moveCursorByY(1);           
                        _windowUpdateHandler->setCursorX(0);
                        
                        list.pushBack('\n'); 
                    //Arrows
                    case 0x25: //LEFT
                        if(_windowUpdateHandler!= nullptr){
                            _windowUpdateHandler->moveCursorByX(-1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x26: //UP
                        if(_windowUpdateHandler != nullptr){
                        _windowUpdateHandler->moveCursorByY(-1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x27: //RIGHT
                        if(_windowUpdateHandler != nullptr){
                        _windowUpdateHandler->moveCursorByX(1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x28: //DOWN
                        if(_windowUpdateHandler != nullptr){
                        _windowUpdateHandler->moveCursorByY(1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                }
                //Normal button clicked
                if(key >= 0x30 && key <= 0x5A){
                    WCHAR tmp = getCharFromWparam(key);
                    list.pushBack(tmp);  
                    _windowUpdateHandler->moveCursorByX(1);
                }
                }catch(const SXException& e){
                    MessageBoxA(hwnd,e.what(),"Error occured", MB_ICONERROR|MB_OK);
                }
            }
        KeyActionHandler(){
            _windowUpdateHandler = nullptr;
        }
        KeyActionHandler(UpdateHandler* _windowUpdateHandler){
           this->_windowUpdateHandler = _windowUpdateHandler; 
        }    
    };


}
