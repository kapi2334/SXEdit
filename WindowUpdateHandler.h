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
            void moveCursorByX(int x){
                int position = x * (_localGrid->getCellWidth());
                _cursor -> moveCursorByX(position);
            }
            //Moves cursor by given Y. Cursor position is relative to grid, witch means y = 1 leads to cursor movement down by 1 cell.
            void moveCursorByY(int y){
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
