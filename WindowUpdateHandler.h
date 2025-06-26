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
                std::cout << "Text: ";
                for(int i = 0; i < inputList.getSize(); i++){
                    char buffer = inputList.get(i);
                    if(buffer == inputList.errorChar) break;
                    tmp[0] = buffer;
                    tmp[1] = '\0';
                    SxPosition pos = _localGrid->calculatePos(i,fontHandler);
                    TextOutA(deviceHandle,pos.x,pos.y,tmp,1);
                    std::cout << tmp;
                }
                std::cout << "|\n";
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
                    drawLetters(textInputList, deviceHandle, font);
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
