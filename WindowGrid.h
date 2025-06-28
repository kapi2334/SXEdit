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
            CursorHandler* _cursor = nullptr;
            FontHandler* _cachedFont = nullptr;
            //Saves index of last char in line
            std::vector<int> _charsInLine = std::vector<int>();
            void updateWindowSizes(){
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
                //Calculating effective window size
                _windowmaxX = _windowRect.right - (_windowRect.left + 2*_cursor->offsetX);
            }
        public:

            SxGrid(HWND hwnd, CursorHandler*& _cursor, FontHandler*& font){
                _windowHandle = hwnd;
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
                this->_cursor = _cursor;
                if (this->_cursor == nullptr){
                    throw new SXException("Failed to pass _cursor pointer to windowGrid service...", _windowHandle);
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
                    outX = _cursor->offsetX;
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
            //Returns position of char on screen by given index 
            SxPosition calculateCurrentPosition(int index){
                updateFont(_cachedFont);
                updateWindowSizes();
                int gridCell = (_cachedFont->getSize()+_cachedFont->_spaceBetweenChars);
                int tmp = 0;
                
                for(int i = 0; i < _charsInLine.size(); i++){
                    if(index > _charsInLine[i]){
                        tmp = i;
                    }
                }
                SxPosition out = SxPosition(((index - tmp)*gridCell) + _cursor->offsetX, (tmp * gridCell) + _cursor->offsetY);
                return out;

            }
            //Returns position of the character in the line (f.e in 'abc' 'b' is 2nd character)
            int calculateLinePosition(SxPosition position){
                int gridCell = (_cachedFont->getSize()+_cachedFont->_spaceBetweenChars);
                return (position.x/gridCell);
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

