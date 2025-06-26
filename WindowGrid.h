#pragma once
#include <Windows.h>
#include "SXException.h"
#include "CursorHandler.h"
#include "FontHandler.h"

namespace sxEditCore{
    class SxPosition{
        public:
            int x;
            int y;
            SxPosition(int x = 0, int y = 0){
                this->x = x;
                this->y = y;
            }
    }; 
    class SxGrid {
        private:
            int _windowmaxX = 0;
            RECT _windowRect;
            HWND _windowHandle;
            CursorHandler* cursor = nullptr;
            void updateWindowSizes(){
                //Calculating effective window size
                _windowmaxX = _windowRect.right - (_windowRect.left + 2*cursor->offsetX);
            }
        public:

            SxGrid(HWND hwnd, CursorHandler*& cursor){
                _windowHandle = hwnd;
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
                this->cursor = cursor;
                if (this->cursor == nullptr){
                    throw new SXException("Failed to pass cursor pointer to windowGrid service...", _windowHandle);
                }
            }
            //Calculates position on screen depending on font settings and postion in list
            SxPosition calculatePos(int positionInList, FontHandler*& font){
                //Update window position
                updateWindowSizes(); 
                //Initial position
                int outX = 0;
                int outY = 0;
                int gridCell = (font->getSize()+font->_spaceBetweenChars);  
                //Adding offset
                outX += cursor->offsetX;
                outX += gridCell * positionInList;
                //Text wrapping
                if(outX >= _windowmaxX){
                    outY = outX/_windowmaxX;
                    outX -= _windowmaxX *outY;
                    outX = outX/gridCell;
                    //Scaling out
                    outX *= gridCell;
                    outX += cursor->offsetX;
                    outY *= gridCell;
                }
                SxPosition tmp(outX, outY);
                return tmp;
            }

    };

}

