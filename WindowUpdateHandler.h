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
                    }
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
                }else if(newCursorPosition.x < 0 && _localGrid->getNumberOfCharsInGivenLine(line-1,list) > 0){
                    //Move cursor up when upper line has chars and user is hitting left arrow in a wall
                    setCursorX(_localGrid->getNumberOfCharsInGivenLine(line-1,list));
                    setCursorY(line-1);
                }
                _cursor -> moveCursorByX(shift);
                _cachedXCursorPosition = _cursor->getXPosition()/_localGrid->getCellWidth();
            }
            //Moves cursor by given Y. Cursor position is relative to grid, witch means y = 1 leads to cursor movement down by 1 cell.
            void moveCursorByY(int y, dataStructures::dlList* list = nullptr){
                int shift = y * _localGrid->getCellWidth();
                SxPosition newCursorPosition = SxPosition(_cursor->getXPosition(),_cursor->getYPosition()+shift);
                int positionInLine = _localGrid->calculateLinePosition(newCursorPosition);
                int line = _cursor->getYPosition()/_localGrid->getCellWidth();
                if (list == nullptr) throw new SXException("Unable to get valid pointer to the memory list to check if cursor movement is valid.", _windowHandle);
                std::cout<<"\nPosition in line: " << positionInLine << "\nNumber of chars in line: " << _localGrid->getNumberOfCharsInGivenLine(line+y, list) << "\n";
                if(positionInLine > _localGrid->getNumberOfCharsInGivenLine(line+y, list)){
                    _cachedXCursorPosition = newCursorPosition.x/_localGrid->getCellWidth();
                    //Set cursor position at the end of the line
                    setCursorX(_localGrid->getNumberOfCharsInGivenLine(line+y, list));
                    setCursorY(line+y);
                    _isCursorXSwitched = true;
                    return;
                }else{
                    if(_isCursorXSwitched){
                        setCursorX(_cachedXCursorPosition);
                    }else{
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
