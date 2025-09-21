#pragma once

#include <Windows.h>
#include "CursorHandler.h"
#include "Data structures/DoublyLinkedList.h"
#include "FontHandler.h"
#include "WindowGrid.h"

namespace sxEditCore{

    class UpdateHandler{

        private:
            HWND _windowHandle;
            SxGrid* _localGrid = nullptr; //Text grid
            CursorHandler* _cursor = nullptr;
            int _spaceBetweenCursorAndLetter = 2;
            bool _debugMode = true;

            // When the cursor moves to the next line, its X position remains the same.
            // The only case when it changes is if the same X position can't be set on the new line (e.g., the new line is shorter).
            // This variable stores the last cursor X position.
            // This position is later used to restore the cursor's X when the line is long enough.
            int _cachedXCursorPosition = 0;
            //Indicates if the cursor's X position changed in the previous function call.
            bool _isCursorXSwitched = false;

            void drawCursor(HDC& hdc){
                _cursor->setCursorHeight(fontHandler->getSize());
                _cursor->drawCursor(hdc); 
            }
            void drawLetters(dataStructures::dlList& inputList,HDC& deviceHandle){
                HFONT font = fontHandler->getFont();
                //Conversion to null-terminated string
                char tmp[2] = { 0 };
                //Setting new font
                HFONT oldFont = (HFONT) SelectObject(deviceHandle,font);
                //Setting up colors
                SetTextColor(deviceHandle, fontHandler->getTextColor()); 
                SetBkMode(deviceHandle, TRANSPARENT); //Transparent text background

                //Writing out text
                SxPosition pos = SxPosition(_cursor->offsetX,_cursor->offsetY);
                _localGrid->resetCharsInLineInformation();
                for(int i = 0; i < inputList.getSize(); i++){
                    bool moveText = false; //Is '\n'
                    char buffer = inputList.get(i);
                    if(buffer == inputList.errorChar) break;
                    if(buffer == '\n'){
                        moveText = true;
                        if(_debugMode == true) buffer = 'N';
                    }
                    if(buffer == ' ' && _debugMode == true) buffer = 'S';
                    tmp[0] = buffer;
                    tmp[1] = '\0';
                    //SxPosition pos = _localGrid->calculatePos(i,fontHandler);
                    
                    TextOutA(deviceHandle,pos.x,pos.y,tmp,1);
                    pos = _localGrid->calculateNextPosition(pos,fontHandler, moveText);
                }
                //Setting back old font
                SelectObject(deviceHandle, oldFont);
            }
            void redraw(HWND& windowHandle){
                InvalidateRect(windowHandle,NULL,true);
                UpdateWindow(windowHandle);
            }
            void writeOutDebug(dataStructures::dlList& list){
                std::cout<<"Cursor data:\n"
                    << "X:" << _cursor->getXPosition() << 
                    "\nY:" << _cursor->getYPosition() << 
                    "\n Text info:" << 
                    "\n List size: " << list.getSize()<<
                    "\n Error char: " << list.errorChar <<
                    "\n End of stream.\n\n";
            }
        public:
            FontHandler* fontHandler = nullptr;
            //Default constructor.
            UpdateHandler(HWND& windowHandle){
                _windowHandle = windowHandle;
                fontHandler = new FontHandler("Arial",12);
                if(fontHandler == nullptr){
                   throw new SXException("Error occured during font creation in WindowUpdateHandler.", _windowHandle); 
                }
                _cursor = new CursorHandler(windowHandle); 
                if(_cursor != nullptr){
                    _localGrid = new SxGrid(windowHandle, _cursor, fontHandler);
                }else{
            throw new SXException("Error occured during cursor creation.", _windowHandle);
                }
            }
            //Destructor
            ~UpdateHandler(){
                delete _localGrid;
                delete fontHandler;
                delete _cursor;
            }
            //Moves cursor by given X. Cursor position is relative to grid, witch means x = 1 leads to cursor movement to the right by 1 cell.
            void moveCursorByX(int x, dataStructures::dlList* list = nullptr){
                int shift = x * (_localGrid->getCellWidth());
                SxPosition newCursorPosition = SxPosition(_cursor->getXPosition()+shift,_cursor->getYPosition());
                int positionInLine =_localGrid->calculateLinePosition(newCursorPosition);
                int line = _cursor->getYPosition()/_localGrid->getCellWidth();
                if (list == nullptr) throw new SXException("Unable to get valid pointer to the memory list to check if cursor movement is valid.", _windowHandle);
                //New cursor position is outside the number of chars in single line.
                if(positionInLine > _localGrid->getNumberOfCharsInGivenLine(line, list)){
                        if(_localGrid->getNumberOfCharsInGivenLine(line+1,list) > 0){
                            setCursorX(0);
                            setCursorY(line+1);
                        }
                    return;
                //}else if(newCursorPosition.x < 0 && _localGrid->getNumberOfCharsInGivenLine(line-1,list) > 0){
                } else if(newCursorPosition.x < 0 && line - 1 >= 0){
                    //If the upper line contains characters and the user presses the left arrow key, move the cursor up
                    setCursorX(_localGrid->getNumberOfCharsInGivenLine(line-1,list)+1);
                    setCursorY(line-1);
                }
                _cursor -> moveCursorByX(shift);
                _cachedXCursorPosition = _cursor->getXPosition()/_localGrid->getCellWidth();
            }
            //Moves the cursor by the given Y.The cursor position is relative to grid, which means y = 1 moves the cursor down by 1 cell.
            void moveCursorByY(int y, dataStructures::dlList* list = nullptr){

                if (list == nullptr) throw new SXException("Unable to get valid pointer to the memory list to check if cursor movement is valid.", _windowHandle);

                //Calculating new cursor position
                int shift = y * _localGrid->getCellWidth();
                SxPosition newCursorPosition = SxPosition(_cursor->getXPosition(),_cursor->getYPosition()+shift);

                //Calculating line data: position of the cursor in a new line and the line number 
                int positionInLine = _localGrid->calculateLinePosition(newCursorPosition);
                int line = _cursor->getYPosition()/_localGrid->getCellWidth();
                //Calculating number of characters in the targetted line 
                int charsInNewLine = _localGrid->getNumberOfCharsInGivenLine(line+y, list);
                

                //Move the cursor to the next line only when last character in the previous line is '\n'
                int numberOfCharsInPrevoiusLine = _localGrid->getNumberOfCharsInGivenLine(line+y-1,list);
                if(numberOfCharsInPrevoiusLine == 0 && y > 0) return; //If last line is empty - dont let move cursor to the other one. 
                int index = _localGrid->calculateScaledGridIndex(numberOfCharsInPrevoiusLine, line+y-1) ;
                index--;
                if(index == -1) {
                    return;
                }
                std::cout << index << " char is: " << list->get(index) << "/" << _localGrid->getNumberOfCharsInGivenLine(line+y-1,list) << "\n";
                if(list->get(index) !='\n' && shift > 0){
                    std::cout << "Last character wasnt new line - returned.";
                    return;
                }
                //If current X position, would result in showing the cursor outside of text area - set the cursor X to the last character's X.
                if(positionInLine > charsInNewLine){
                    //Save last cursor position to cache.
                    _cachedXCursorPosition = newCursorPosition.x/_localGrid->getCellWidth();
                    //Set cursor position at the end of the line
                    setCursorX(_localGrid->getNumberOfCharsInGivenLine(line+y, list));
                    setCursorY(line+y);
                    _isCursorXSwitched = true;
                    return;
                }else{
                    //If X position was switched last time, set current position to cashed value.
                    if(_isCursorXSwitched){
                        setCursorX(_cachedXCursorPosition);
                    }else{
                        //If not, save current X position to cashe.
                        _cachedXCursorPosition = newCursorPosition.x/_localGrid->getCellWidth();
                    }
                    _isCursorXSwitched = false;
                }
                _cursor->moveCursorByY(y*(_localGrid->getCellWidth()));
            }
            //Sets cursor on given position on X axis. The position is relative to the grid.
            void setCursorX(int x){
                _cursor->updateCursorX(x*(_localGrid->getCellWidth()));
            }
            //Sets cursor on given position on Y axis. The position is relative to the grid.
            void setCursorY(int y){
                _cursor->updateCursorY(y*(_localGrid->getCellWidth()));
            }
            //Applies next available style to the cursor.
            void switchCursorToNextStyle(){
                _cursor->switchCursorStyle();
            }
            //Applies specified style to the cursor.
            void switchCursorStyle(int style){
                _cursor->switchCursorStyle(style);
            }
            //Returns the index of the letter the cursor is currently on.
            int getIndexOfActiveLetter(){
                    SxPosition currentCursorPos = SxPosition(_cursor->getXPosition(), _cursor->getYPosition());
                    int index = _localGrid->calculateCachedIndex(currentCursorPos);
                    return index;
                return 1;
            }
            //Returns position in current line of letter with given index.
            int getLinePositionOfIndexedLetter(int index){
                SxPosition positionOfIndex = _localGrid->calculateCurrentPosition(index);
                return _localGrid->calculateLinePosition(positionOfIndex);
            }
            
            //Updates all data on screen.
            void Update(dataStructures::dlList& textInputList, PAINTSTRUCT& ps){
                if(_windowHandle){
                    HDC deviceHandle = BeginPaint(_windowHandle,&ps);
                    drawLetters(textInputList, deviceHandle);
                    drawCursor(deviceHandle);
                    EndPaint(_windowHandle,&ps);
                    redraw(_windowHandle);
                    _localGrid->updateFont(fontHandler);
                    _localGrid->updateWindowMaxSizes();
                    //writeOutDebug(textInputList);
                }else{
                    throw new SXException("Fatal Error from windowUpdateHandler: Unable to get valid windowHandle");
                }


            }

    };


}
