#include <Windows.h>
#include <iostream>
#include <winuser.h>
#include "SXException.h"
namespace sxEditCore{
    class CursorHandler{
        private:
            int x = 0;
            int y = 0;
            int type = 0;
            /* 0 = |
             * 1 = _
             * */
            const int width = 2;
            const int height = 18;
            const int offsetX = 10;
            const int offsetY = 10;
            int _windowmaxX = 0;
            int _windowmaxY = 0;
            HWND  _windowHandle;
            RECT _windowRect; //Client field react
                               //
            void writeOutDebug(){
                std::cout<<"Cursor data:\n"
                    << "X:" << x << 
                    "\nY:" << y << 
                    "\nWidth:" << width << 
                    "\nHeight:" << height << 
                    "\nRealX: "<< x + offsetX << 
                    "\nRealY:" << y + offsetY << 
                    "\nWindowWidth" << _windowmaxX <<
                    "\nWindowHeight" << _windowmaxY;
            }
            void updateWindowSizes(){
                //Calculating effective window size
                _windowmaxX = _windowRect.right - (_windowRect.left + width + 2*offsetX);
                _windowmaxY = _windowRect.bottom - (_windowRect.top + height + offsetY);
            }
        public:
            CursorHandler(HWND& windowHandle){
                this->_windowHandle = windowHandle;
                if(_windowHandle == nullptr) {
                    std::cout << "Fatal Error: Window handler passed to CurosrHandler function is invalid.";
                }
                if(!GetClientRect(_windowHandle, &_windowRect)){
                    throw new SXException("Failed to get client field...", windowHandle); 
                }
                std::cout<<"Creating cursor...\n";
            }


            //Sets new X param 
            bool updateCursorX(int newX){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                int effectiveNewX = newX*width;
                //Checking if cursor will not go out of the window filed.
                if( effectiveNewX >= 0 && effectiveNewX <= _windowmaxX-width-offsetX){
                    x = newX*width;
                    return true;
                } 
                return false;
            } 

            //Sets new Y param 
            bool updateCursorY(int newY){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                int effectiveNewY = newY * height; 
                //Checking if cursor will not go out of the window filed.
                if( effectiveNewY >= 0 && effectiveNewY <= _windowmaxY){
                    y = effectiveNewY;
                    return true;
                }
                return false;
            } 

            //Moves cursor by X value
            bool moveCursorByX(int x){
                //Making sure window sizes collected in variables are up to date.
                updateWindowSizes();
                int effectiveNewX =  this->x + (x*width);
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
                int effectiveNewY =  this->y + (y*height);
                //Checking if cursor will not go out of the window filed.
                if( effectiveNewY >= 0 && effectiveNewY <= _windowmaxY){
                    std::cout<<"\n-------------Effective new Y: " << effectiveNewY<<"\n";
                    this->y = effectiveNewY; 
                    return true;
                }
                return false;
            }
            int getXPosition(){
                return x;
            }
            int getYPosition(){
                return y;
            }
            void redraw(){
                InvalidateRect(_windowHandle,NULL,true);
                UpdateWindow(_windowHandle);
            }

            void drawCursor(PAINTSTRUCT ps){
                std::cout<<"Drawing cursor...";
                writeOutDebug();
                HDC deviceHande = BeginPaint(_windowHandle, &ps);
                HPEN cursorPen = CreatePen(PS_SOLID, 1,RGB(255,255,255));
                HPEN oldPen = (HPEN) SelectObject(deviceHande, cursorPen);
                switch(type){
                    case 0:{
                    Rectangle(deviceHande,x+offsetX,y+offsetY,(width+x+offsetX),(height+y+offsetY));
                    break;
                    }
                    case 1:{
                    Rectangle(deviceHande,x+offsetX,y+offsetY+(height-(width+x+offsetX)),(height+y+offsetY),(width+x+offsetX));
                    break;
                    }
                }
                SelectObject(deviceHande, oldPen);
                DeleteObject(cursorPen);
                EndPaint(_windowHandle, &ps);
                redraw();
            }
            void drawLetter(char input,PAINTSTRUCT ps, HFONT& font){
                //Conversion to null-terminated string
                char tmp[2] = {input, '\0'};
                HDC deviceHandle = BeginPaint(_windowHandle,&ps);
                //If passed font is invalid, create new one 
                if(font == NULL){
                    throw new SXException("Font passed to function is invalid, used generic one insted.",_windowHandle);
                }
                //Setting new font
                HFONT oldFont = (HFONT) SelectObject(deviceHandle,font);
                //Writing out text
                TextOutA(deviceHandle,x,y,tmp,1);
                //Setting back old font
                SelectObject(deviceHandle, oldFont);
                EndPaint(_windowHandle,&ps);
                //Move cursor to next position.
                moveCursorByX(1);
            }
    };


}


