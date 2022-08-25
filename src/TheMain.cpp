#include "TheMain.h"
#include "./util/render.h"

#include <windows.h>

LRESULT screen_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // switch (msg) {
    //     case WM_CLOSE:
    //         screen_exit = 1;
    //         break;
    //     case WM_KEYDOWN:
    //         screen_keys[wParam & 511] = 1;
    //         break;
    //     case WM_KEYUP:
    //         screen_keys[wParam & 511] = 0;
    //         break;
    //     default:
    //         return DefWindowProc(hWnd, msg, wParam, lParam);
    // }
    return 0;
}
//
//int main() {
//    std::cout << "[APP START]" << std::endl;
//
//    HINSTANCE hInstance = GetModuleHandle(NULL);
//    LPCWSTR className = TEXT("SCREEN3.1415926");
//    LPCWSTR windowTitle = TEXT("测试窗口");
//
//    WNDCLASSEX wc = {
//        sizeof(WNDCLASSEX),
//        CS_BYTEALIGNCLIENT,
//        (WNDPROC)screen_events,
//        0,
//        0,
//        hInstance,
//        LoadIcon(NULL, IDI_APPLICATION),
//        LoadCursor(NULL, IDC_ARROW),
//        (HBRUSH)GetStockObject(BLACK_BRUSH),
//        NULL,
//        className,
//        LoadIcon(NULL, IDI_APPLICATION)
//    };
//
//    if (!RegisterClassEx(&wc)) return -1;
//
//    HWND screen_handle = CreateWindowEx(
//        0L,
//        className,
//        windowTitle,
//        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        NULL,
//        NULL,
//        hInstance,
//        NULL
//    );
//
//    if (screen_handle == NULL) return GetLastError();
//
//    ShowWindow(screen_handle, SW_NORMAL);
//
//
//    std::cout << "[APP END]" << std::endl;
//    return 0;
//}

// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// the entry point for any Windows program
int main() {
    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    //WNDCLASSEX wc;

    //// clear out the window class for use
    //ZeroMemory(&wc, sizeof(WNDCLASSEX));

    //// fill in the struct with the needed information
    //wc.cbSize = sizeof(WNDCLASSEX);
    //wc.style = CS_HREDRAW | CS_VREDRAW;
    //wc.lpfnWndProc = WindowProc;
    //wc.hInstance = GetModuleHandle(NULL);
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    //wc.lpszClassName = TEXT("WindowClass1");


    /////////////
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_BYTEALIGNCLIENT,
        WindowProc,
        0,
        0,
        GetModuleHandle(NULL),
        LoadIcon(NULL, IDI_APPLICATION),
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)GetStockObject(BLACK_BRUSH),
        NULL,
        TEXT("WindowClass1"),
        LoadIcon(NULL, IDI_APPLICATION)
    };
    /////////////

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(0,
                          TEXT("WindowClass1"),  // name of the window class
                          TEXT("Hello, Engine!"), // title of the window
                          WS_OVERLAPPEDWINDOW,  // window style
                          300,                  // x-position of the window
                          300,                  // y-position of the window
                          500,                  // width of the window
                          400,                  // height of the window
                          NULL,                 // we have no parent window, NULL
                          NULL,                 // we aren't using menus, NULL
                          GetModuleHandle(NULL), // application handle
                          NULL);                // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd, SW_NORMAL);

    // enter the main loop:

    // this struct holds Windows event messages
    MSG msg;

    // wait for the next message in the queue, store the result in 'msg'
    while (GetMessage(&msg, NULL, 0, 0)) {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);

        // send the message to the WindowProc function
        DispatchMessage(&msg);
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // sort through and find what code to run for the message given
    switch (message) {
    // this message is read when the window is closed
    case WM_DESTROY: {
        // close the application entirely
        PostQuitMessage(0);
        return 0;
    } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}
