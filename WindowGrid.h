#pragma once
#include <Windows.h>
#include "SXException.h"
#include "CursorHandler.h"
#include "FontHandler.h"
#include "SxPosition.h"
namespace sxEditCore{
    class SxGrid {
        private:
            int _windowmaxX = 0;
            RECT _windowRect;
            HWND _windowHandle;
            CursorHandler* cursor = nullptr;
            FontHandler* _cachedFont = nullptr;
            void updateWindowSizes(){
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
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
            //Returns position of a next cell, when wrap is true - shifts next postion to new line.
            SxPosition calculateNextPosition(SxPosition currentPostion, FontHandler*& font, bool wrap = false){
                updateWindowSizes();
                int outX = currentPostion.x;
                int outY = currentPostion.y;
                int gridCell = (font->getSize()+font->_spaceBetweenChars);

                outX += gridCell;       
                if(outX >= _windowmaxX || wrap == true){
                    outX = cursor->offsetX;
                    outY += gridCell; 
                }
                currentPostion.x = outX;
                currentPostion.y = outY;
                return currentPostion;

            }
            //Returns index of char in list, based on position on screen
            int calculateIndex(SxPosition position, FontHandler*& font){
                //Save font in cache
                _cachedFont = font; 
                //Get new window size
                updateWindowSizes();
                //Calculate single cell width
                int cellDefinition = (font->getSize()+ font->_spaceBetweenChars);
                return (position.x/cellDefinition) + position.y * (_windowmaxX/cellDefinition);

            }
            void updateFont(FontHandler*& font){
                _cachedFont = font;
            }
            int getCellWidth(){
                return _cachedFont->getSize() + _cachedFont->_spaceBetweenChars;
            }

    };

}

