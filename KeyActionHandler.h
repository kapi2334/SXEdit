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

                if(!(_shouldBeCapital())){ 
                    tmp[0] = towlower(tmp[0]);
                }
                    
                return *tmp;
            }
            bool checkIsKeyValid(WPARAM key){
                if(key >= 0x30 && key <= 0x5A && key != 0x40) return true; //Normal keys
                if(key >= 0x60 && key <= 0x69){ //numpad 1-9 keys
                    if((GetKeyState(VK_NUMLOCK) & 0x0001) != 0) return true; //Numlock is active 
                }
                if(key >= 0x6A && key <= 0x6F) return true; // numpad '+','-','*','/' keys
                return false;
            }
            //Returns true if in current moment SHIFT key is pressed or CAPS is active.
            bool _shouldBeCapital(){
                //Checking if SHIFT or CAPS are pressed
                bool shift = (GetKeyState(VK_SHIFT)&0x8000) != 0; //Checking if 15th bit = 1 - it means that shift is pressed.
                bool caps = (GetKeyState(VK_CAPITAL)&0x0001) !=0; //Checking if first bit = 1 - it means that shift is pressed.
                return (shift ^ caps); //XOR
            }
            //Returns true if in current moment ctrl key is pressed.
            bool _isCtrlPressed(){
                return (GetKeyState(VK_CONTROL) & 0x8000);
            }
        public:
            //By this amount the font size is changed when you click 'ctrl' + '+/-'
            int fontResizingFactor = 2;
            void registerPress(WPARAM key,dataStructures::dlList& list, HWND hwnd){
                try{
                switch(key){
                    //case 0x09: //TAB
                    case 0x20: //SPACEBAR
                        list.pushAtIndex(_windowUpdateHandler->getIndexOfActiveLetter(), ' ');
                       _windowUpdateHandler->moveCursorByX(1,&list);
                       break;
                    case 0x2D: //INSERT
                        //With ctrl - allows you to switch between different cursor styles.
                        //Without - changes cursor input style - normal/overwrite
                        if(_isCtrlPressed()){
                            _windowUpdateHandler->switchCursorToNextStyle();                            
                        }else{

                        }
                        break;
                    case 0x2E: //DELETE
                        list.deleteNode(_windowUpdateHandler->getIndexOfActiveLetter());
                        break;
                    case 0x08: //Backspace
                        list.deleteNode(_windowUpdateHandler->getIndexOfActiveLetter()-1);
                        _windowUpdateHandler->moveCursorByX(-1, &list);
                        break;
                    case 0x0D: //ENTER
                        list.pushAtIndex(_windowUpdateHandler->getIndexOfActiveLetter(), '\n');
                        _windowUpdateHandler ->moveCursorByY(1, &list);           
                        _windowUpdateHandler->setCursorX(0);
                        break;
                    //Arrows
                    case 0x25: //LEFT
                        if(_windowUpdateHandler!= nullptr){
                            _windowUpdateHandler->moveCursorByX(-1, &list); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x26: //UP
                        if(_windowUpdateHandler != nullptr){
                            _windowUpdateHandler->moveCursorByY(-1, &list); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x27: //RIGHT
                        if(_windowUpdateHandler != nullptr){
                            _windowUpdateHandler->moveCursorByX(1,&list); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x28: //DOWN
                        if(_windowUpdateHandler != nullptr){
                            _windowUpdateHandler->moveCursorByY(1,&list); //one to the left
                        }else{
                            throw SXException("Fatal error occurred while trying to get cursor handler: handler does not exist.");   
                        }
                        break;
                    case 0x6B: // + on numpad    
                    case 0xBB: // +
                        if(_isCtrlPressed()){ //Is ctrl key pressed
                                                              //Make font bigger by fontResizingFactor 
                            //Save on which letter in current line cursor is on. 
                            int currentActiveLetterIndex =_windowUpdateHandler->getLinePositionOfIndexedLetter(
                                    _windowUpdateHandler->getIndexOfActiveLetter()
                                    );
                            //Update font size
                            int currentFontSize = _windowUpdateHandler->fontHandler->getSize();
                            _windowUpdateHandler->fontHandler->updateFontSize(currentFontSize + fontResizingFactor);
                            //Set cursor in valid position on screen.
                            _windowUpdateHandler->setCursorX(currentActiveLetterIndex);
                        }
                        break;
                    case 0x6D: // - on numpad    
                    case 0xBD: // -
                        if(_isCtrlPressed()){ //Is ctrl key pressed
                                                              //Make font smaller by fontResizingFactor 
                            //Save on which letter in current line cursor is on. 
                            int currentActiveLetterIndex =_windowUpdateHandler->getLinePositionOfIndexedLetter(
                                    _windowUpdateHandler->getIndexOfActiveLetter()
                                    );
                            //Update font size
                            int currentFontSize = _windowUpdateHandler->fontHandler->getSize();
                            _windowUpdateHandler->fontHandler->updateFontSize(currentFontSize - fontResizingFactor);
                            //Set cursor in valid position on screen.
                            _windowUpdateHandler->setCursorX(currentActiveLetterIndex);
                        }
                        break;
                    //Normal button clicked    
                    default:
                        if(checkIsKeyValid(key)){ 
                            WCHAR tmp = getCharFromWparam(key);
                            //Pushing letter at active index.
                            list.pushAtIndex(_windowUpdateHandler->getIndexOfActiveLetter(), tmp);
                            _windowUpdateHandler->moveCursorByX(1, &list);
                            InvalidateRect(hwnd, NULL, true);
                            UpdateWindow(hwnd);
                        }
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
