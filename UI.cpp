#include <windows.h> 

#define ITEM_QUIT  1001
#define ITEM_ABOUT 1002
//коды, отсылающие к кнопкам Quit/About

void add_menu_bar(HWND hwnd)
{
    HMENU hFileMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();
    HMENU hQuitItem = CreateMenu();
    HMENU hAboutItem = CreateMenu();

    AppendMenu(hQuitItem, MF_STRING, ITEM_QUIT, L"&Quit");
    AppendMenuW(hFileMenu, MF_POPUP, (UINT_PTR)hQuitItem, L"&File");

    AppendMenu(hAboutItem, MF_STRING, ITEM_ABOUT, L"&About");
    AppendMenuW(hFileMenu, MF_POPUP, (UINT_PTR)hAboutItem, L"&Help");

    SetMenu(hwnd, hFileMenu);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//здесь вся интерактивность
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (message)
    {
    case WM_CREATE://действия при создании окна
        add_menu_bar(hwnd);/* Add Initialization Code */
        return 0;
    case WM_PAINT://сообщение передается, если стала видна перекрытая область окна, если вы изменили размер окна
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        DrawText(hdc, TEXT("This is your first Windows Window!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);//отмена сбора данных с мыши
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"HelloWorldWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Hello, World!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) {
        return 0;
    }
    MSG msg = {};
    ShowWindow(hwnd, nCmdShow);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
