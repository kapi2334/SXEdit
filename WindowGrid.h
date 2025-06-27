#pragma once
#include <Windows.h>
#include "SXException.h"
#include "CursorHandler.h"
#include "FontHandler.h"
#include "SxPosition.h"
#include <vector>
namespace sxEditCore{
    class SxGrid {
        private:
            int _windowmaxX = 0;
            RECT _windowRect;
            HWND _windowHandle;
            CursorHandler* cursor = nullptr;
            FontHandler* _cachedFont = nullptr;
            //Saves index of last char in line
            std::vector<int> _charsInLine = std::vector<int>();
            void updateWindowSizes(){
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
                //Calculating effective window size
                _windowmaxX = _windowRect.right - (_windowRect.left + 2*cursor->offsetX);
            }
        public:

            SxGrid(HWND hwnd, CursorHandler*& cursor, FontHandler*& font){
                _windowHandle = hwnd;
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
                this->cursor = cursor;
                if (this->cursor == nullptr){
                    throw new SXException("Failed to pass cursor pointer to windowGrid service...", _windowHandle);
                }
                if(font == nullptr){
                    throw new SXException("Failed to pass vaild font handler to windowGrid service", _windowHandle);
                }
                this->_cachedFont = font;
            }
            //Returns position of a next cell, when wrap is true - shifts next postion to new line.
            SxPosition calculateNextPosition(SxPosition currentPostion, FontHandler*& font, bool wrap = false){
                updateFont(font);
                updateWindowSizes();
                int outX = currentPostion.x;
                int outY = currentPostion.y;
                int gridCell = (font->getSize()+font->_spaceBetweenChars);

                outX += gridCell;       
                if(outX >= _windowmaxX || wrap == true){
                    outX = cursor->offsetX;
                    outY += gridCell; 
                    _charsInLine.push_back(calculateIndex(currentPostion,font)+1);
                }
                currentPostion.x = outX;
                currentPostion.y = outY;
                return currentPostion;

            }
            //Returns index of char in list, based on position on screen
            int calculateIndex(SxPosition position, FontHandler*& font){
                updateFont(font);
                //Save font in cache
                _cachedFont = font; 
                //Get new window size
                updateWindowSizes();
                //Calculate single cell width
                int cellDefinition = (font->getSize()+ font->_spaceBetweenChars);
                int prevLinesIndexes = 0;
                if(!_charsInLine.empty()){
                    //Gets index of single char and add previous indexes from previous lines
                    prevLinesIndexes = _charsInLine[(position.y/cellDefinition)-1];
                    if(position.y == 0 ) prevLinesIndexes = 0;
                    std::cout << prevLinesIndexes;
                }
                return (position.x/cellDefinition) + prevLinesIndexes;

            }

            //Returns index of char in list, based on position on screen. Uses cashed font.
            int calculateCachedIndex(SxPosition position){
                return calculateIndex(position, _cachedFont);
            }
            void updateFont(FontHandler*& font){
                _cachedFont = font;
            }
            //Resets information about number of characters in one line.
            void resetCharsInLineInformation(){
                _charsInLine.clear(); 
            }
            int getCellWidth(){
                if(_cachedFont == nullptr) {
                    throw new SXException("Unable to get valid font handler", _windowHandle);
                }
                else{
                    return _cachedFont->getSize() + _cachedFont->_spaceBetweenChars;
                }
            }

    };

}

