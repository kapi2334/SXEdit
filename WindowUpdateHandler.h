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
                _cursor->drawCursor(hdc); 
            }
            void drawLetters(HWND& windowHandle, dataStructures::dlList& inputList,HDC& deviceHandle,FontHandler*& fontHandler){
                HFONT font = fontHandler->getFont();
                //Conversion to null-terminated string
                char tmp[2] = { 0 };
                //If passed font is invalid, create new one 
                if(font == NULL){
                    throw new SXException("Font passed to function is invalid, used generic one insted.",_windowHandle);
                }
                //Setting new font
                HFONT oldFont = (HFONT) SelectObject(deviceHandle,font);
                //Setting up colors
                SetTextColor(deviceHandle, fontHandler->getTextColor()); 
                SetBkMode(deviceHandle, TRANSPARENT); //Transparent text background

                //Writing out text
                for(int i = 0; i < inputList.size; i++){
                    char buffer = inputList.get(i);
                    if(buffer == inputList.errorChar) break;
                    tmp[0] = buffer;
                    SxPosition pos = _localGrid->calculatePos(i,fontHandler);
                    //std::cout << "\n========\n" << "Text graphic position:\n X:  " << pos.x << "\nY: " << pos.y << "\n =============";
                    TextOutA(deviceHandle,pos.x,pos.y,tmp,1);
                    //Move cursor to next position.
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
                    "\n List size: " << list.size <<
                    "\n Error char: " << list.errorChar <<
                    "\n End of stream.\n\n";
            }
        public:
            UpdateHandler(HWND& windowHandle, CursorHandler*& cursor){
                _windowHandle = windowHandle;
                _cursor = cursor; 
                _localGrid = new SxGrid(windowHandle, cursor);
            }
            ~UpdateHandler(){
                delete _localGrid;
            }

            void Update(dataStructures::dlList& textInputList, PAINTSTRUCT& ps,FontHandler*& font){
                if(_windowHandle){
                    HDC deviceHandle = BeginPaint(_windowHandle,&ps);
                    drawLetters(_windowHandle,textInputList, deviceHandle, font);
                    drawCursor(deviceHandle);
                    EndPaint(_windowHandle,&ps);
                    redraw(_windowHandle);
                    //writeOutDebug(textInputList);
                }else{
                    throw new SXException("Fatal Error from windowUpdateHandler: Unable to get valid windowHandle");
                }


            }

    };


}
