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
            void drawCursor(HDC& hdc,FontHandler*& fontHandler){
                _cursor->setCursorSize(fontHandler->getSize());
                _cursor->drawCursor(hdc); 
            }
            void drawLetters(dataStructures::dlList& inputList,HDC& deviceHandle,FontHandler*& fontHandler){
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
                    //std::cout << tmp;
                }
                //std::cout << "|\n";
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
            UpdateHandler(HWND& windowHandle, FontHandler*& font){
                _windowHandle = windowHandle;
                _cursor = new CursorHandler(windowHandle); 
                if(_cursor != nullptr){
                    _localGrid = new SxGrid(windowHandle, _cursor, font);
                }else{
                    throw new SXException("Error occured during cursor creation.", _windowHandle);
                }
            }
            ~UpdateHandler(){
                delete _localGrid;
                delete _cursor;
            }
            void moveCursorByX(int x){
                std::cout << x * (_localGrid -> getCellWidth());
                _cursor->moveCursorByX(x * (_localGrid->getCellWidth()));
            }
            void moveCursorByY(int y){
                _cursor->moveCursorByY(y*(_localGrid->getCellWidth()));
            }
            void setCursorX(int x){
                _cursor->updateCursorX(x*(_localGrid->getCellWidth()));
            }
            void setCursorY(int y){
                _cursor->updateCursorY(y*(_localGrid->getCellWidth()));
            }
            //Returns the index of the letter the cursor is currently on.
            int getIndexOfActiveLetter(){
                    SxPosition currentCursorPos = SxPosition(_cursor->getXPosition(), _cursor->getYPosition());
                    int index = _localGrid->calculateCachedIndex(currentCursorPos);
                    return index;
                return 1;
            }
            int getLinePositionOfIndexedLetter(int index){
                SxPosition positionOfIndex = _localGrid->calculateCurrentPosition(index);
                return _localGrid->calculateLinePosition(positionOfIndex);
            }
            

            void Update(dataStructures::dlList& textInputList, PAINTSTRUCT& ps,FontHandler*& font){
                if(_windowHandle){
                    HDC deviceHandle = BeginPaint(_windowHandle,&ps);
                    drawLetters(textInputList, deviceHandle, font);
                    drawCursor(deviceHandle,font);
                    EndPaint(_windowHandle,&ps);
                    redraw(_windowHandle);
                    //writeOutDebug(textInputList);
                }else{
                    throw new SXException("Fatal Error from windowUpdateHandler: Unable to get valid windowHandle");
                }


            }

    };


}
