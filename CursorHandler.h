#pragma once

#include <Windows.h>
#include <iostream>
#include "SXException.h"

namespace sxEditCore{
    class CursorHandler{
        private:
            int x = 0;
            int y = 0;
            //Defines active cursor style
            int _type = 0;
            /* 0 = |
             * 1 = _
             * */
            //Defines how many cursor styles are implemented;
            int _numberOfTypes = 2; 

            HWND  _windowHandle;
            RECT _windowRect; //Client field react
            
            void writeOutDebug(std::string calledAt = "Not specified."){
                std::cout << "\n\n--------------------------------\n"; //Separator
                std::cout<<"Cursor data:"<<
                    "\nX:" << x << 
                    "\nY:" << y << 
                    "\nWidth:" << width << 
                    "\nHeight:" << height << 
                    "\nRealX: "<< x + offsetX << 
                    "\nRealY:" << y + offsetY << 
                    "\nWindowWidth" << _windowmaxX <<
                    "\nWindowHeight" << _windowmaxY <<
                    "\nCursorType: " << _type <<
                    "\nNumberOfTypes: " << _numberOfTypes <<
                    "\nDEBUG MESSAGE CALLED IN: " << "CursorHandler.h " << ", at: " << calledAt <<
                    "\nEnd of stream.";
                std::cout << "\n--------------------------------\n"; //Separator
            }
            void updateWindowSizes(){
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", _windowHandle); 
                }
                //Calculating effective window size
                _windowmaxX = _windowRect.right - (_windowRect.left + width + 2*offsetX);
                _windowmaxY = _windowRect.bottom - (_windowRect.top + height + offsetY);
            }
        public:
            int width = 2;
            int height = 18;
            int heaviness = 2; //How thick is _ cursor 
            int offsetX = 10;
            int offsetY = 10;
            int _windowmaxX = 0;
            int _windowmaxY = 0;
            CursorHandler(HWND& windowHandle){
                this->_windowHandle = windowHandle;
                if(_windowHandle == nullptr) {
                    throw new SXException("Fatal Error: Window handler passed to CurosrHandler function is invalid.",_windowHandle);
                }
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", windowHandle); 
                }
            }
            void setCursorHeight(int newHeight){
                this->height = newHeight;
            }



            //Sets new X param 
            bool updateCursorX(int newX){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                int effectiveNewX = newX + offsetX;
                //Checking if cursor will not go out of the window filed.
                if( effectiveNewX >= 0 && effectiveNewX <= _windowmaxX-width){
                    x = effectiveNewX;
                    return true;
                } 
                return false;
            } 

            //Sets new Y param 
            bool updateCursorY(int newY){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                int effectiveNewY = newY ; 
                //Checking if cursor will not go out of the window filed.
                /*if( effectiveNewY >= 0 && effectiveNewY <= _windowmaxY){
                    y = effectiveNewY;
                    return true;
                }*/
                y = effectiveNewY;
                return true;
            } 

            //Moves cursor by X value
            bool moveCursorByX(int x){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                int effectiveNewX = this->x + x;  //this->x + (x*width);
                //Checking if cursor will not go out of the window filed.
                if( effectiveNewX >= 0 && effectiveNewX <= _windowmaxX){
                    this->x = effectiveNewX; 
                    return true;
                }
                return false;
            }

            //Moves cursor by Y value
            bool moveCursorByY(int y){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                //Checking if cursor will not go out of the window filed.
                if((this->y + y) >= 0){
                        this->y += y;
                    return true;
                }else{
                    return false;
                }

            }
            int getXPosition(){
                return x;
            }
            int getYPosition(){
                return y;
            }
            //Sets cursor style to passed type. If no type passed - next valid cursor type will be applied.
            void switchCursorStyle(int style = -1){
                if(style != -1){
                    this->_type = style;
                }else{
                   if((this->_type + 1) > _numberOfTypes-1){
                        this->_type = 0; 
                   }else{
                        this->_type ++;
                   } 
                }
            }

            void drawCursor(HDC& deviceHandle){
                //writeOutDebug();
                HPEN cursorPen = CreatePen(PS_SOLID, 1,RGB(255,255,255));
                HPEN oldPen = (HPEN) SelectObject(deviceHandle, cursorPen);
                switch(_type){
                    case 0:{
                    Rectangle(deviceHandle,x+offsetX,y+offsetY,(width+x+offsetX),(height+y+offsetY));
                    break;
                    }
                    case 1:{
                    Rectangle(deviceHandle,x+offsetX,y+offsetY+height,(x+offsetX)+height,(y+offsetY+height)+heaviness);
                    break;
                    }
                }
                SelectObject(deviceHandle, oldPen);
                DeleteObject(cursorPen);
                //writeOutDebug("End of the drawCursor function.");
            }
    };


}


