#include <Windows.h>
#include "SXException.h"
#include "Window.h" 
#if defined(_WIN64) || defined(_WIN32)

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR,int nCmdShow){
        sxEditCore::WindowsWindowLogic mainWindow = sxEditCore::WindowsWindowLogic(
                hInstance,
                "SXEdit",
                "SX_Main_Window_Class",
                400,400);
        if(!mainWindow.Create()) throw std::runtime_error("Unable to create window (Windows model). App cannot be started...");
        mainWindow.Show(nCmdShow, true);

        MSG msg = {};

        //Window message handling
        while(GetMessage(&msg, nullptr,0,0 )){

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        return 0;
    }

#elif defined(__linux__)
#elif defined(__APPLE__) && defined(__MACH__)
#endif        
