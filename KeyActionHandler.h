#pragma once

#include <Windows.h>
#include "CursorHandler.h"
#include "Data structures/DoublyLinkedList.h"

namespace sxEditCore{
    class KeyActionHandler{
        private:
            CursorHandler* cursorHandler = nullptr;

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
                    //case 0x0D: //ENTER
                    //Arrows
                    case 0x25: //LEFT
                        if(cursorHandler != nullptr){
                            cursorHandler->moveCursorByX(-1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x26: //UP
                        if(cursorHandler != nullptr){
                        cursorHandler->moveCursorByY(-1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x27: //RIGHT
                        if(cursorHandler != nullptr){
                        cursorHandler->moveCursorByX(1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x28: //DOWN
                        if(cursorHandler != nullptr){
                        cursorHandler->moveCursorByY(1); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                }
                //Normal button clicked
                if(key >= 0x30 && key <= 0x5A){
                    WCHAR tmp = getCharFromWparam(key);
                    list.pushBack(tmp);  
                }
                }catch(const SXException& e){
                    MessageBoxA(hwnd,e.what(),"Error occured", MB_ICONERROR|MB_OK);
                }
            }
        KeyActionHandler(){
            cursorHandler = nullptr;
        }
        KeyActionHandler(CursorHandler* cursorHandler){
           std::cout<<"Created keyAction handler.";
           this->cursorHandler = cursorHandler; 
        }    
    };


}
