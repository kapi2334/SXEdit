#pragma once

#include <Windows.h>
#include "CursorHandler.h"
#include "Data structures/DoublyLinkedList.h"
#include "FontHandler.h"


namespace sxEditCore{

    class UpdateHandler{

        private:
            HWND _windowHandle;
            int _spaceBetweenChars = 15; //Put it as font variable
            void drawCursor(CursorHandler*& cursor, HDC& hdc){
                cursor->drawCursor(hdc); 
            }
            void drawLetters(HWND& windowHandle, dataStructures::dlList& inputList,CursorHandler*& cursor,HDC& deviceHandle,FontHandler*& fontHandler){
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
                    TextOutA(deviceHandle,(i*fontHandler->getSize())+_spaceBetweenChars,cursor->getYPosition(),tmp,1);
                    //Move cursor to next position.
                }
                //Setting back old font
                SelectObject(deviceHandle, oldFont);
            }
            void redraw(HWND& windowHandle){
                InvalidateRect(windowHandle,NULL,true);
                UpdateWindow(windowHandle);
            }
            void writeOutDebug(CursorHandler*& cursor, dataStructures::dlList& list){
                std::cout<<"Cursor data:\n"
                    << "X:" << cursor->getXPosition() << 
                    "\nY:" << cursor->getYPosition() << 
                    "\n Text info:" << 
                    "\n List size: " << list.size <<
                    "\n Error char: " << list.errorChar <<
                    "\n End of stream.\n\n";
            }
        public:
            UpdateHandler(HWND& windowHandle){
                _windowHandle = windowHandle;
            }
            void Update(CursorHandler*& cursor,dataStructures::dlList& textInputList, PAINTSTRUCT& ps,FontHandler*& font){
                if(_windowHandle){
                    HDC deviceHandle = BeginPaint(_windowHandle,&ps);
                    drawLetters(_windowHandle,textInputList, cursor, deviceHandle, font);
                    drawCursor(cursor,deviceHandle);
                    EndPaint(_windowHandle,&ps);
                    redraw(_windowHandle);
                    writeOutDebug(cursor,textInputList);
                }else{
                    throw new SXException("Fatal Error from windowUpdateHandler: Unable to get valid windowHandle");
                }


            }

    };


}
