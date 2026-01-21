#include <windows.h>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ChangeResolution(int width, int height);
void LaunchApp(const wchar_t* appPath);
void OpenFile(const wchar_t* filePath);
void CustomAction(const wchar_t* buttonName);

#define BTN_RES_1920 1
#define BTN_RES_1280 2
#define BTN_LAUNCH_APP 3
#define BTN_OPEN_FILE 4
#define BTN_5 5
#define BTN_6 6
#define BTN_7 7
#define BTN_8 8

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"StreamDeckClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Virtual Stream Deck",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 420, 220,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    int btnWidth = 90, btnHeight = 60;
    int startX = 10, startY = 10;
    int spacingX = 100, spacingY = 70;

    CreateWindow(L"BUTTON", L"1920x1080",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX, startY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_RES_1920, hInstance, NULL);

    CreateWindow(L"BUTTON", L"1720x1080",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX + spacingX, startY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_RES_1280, hInstance, NULL);

    CreateWindow(L"BUTTON", L"Notepad",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX + spacingX * 2, startY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_LAUNCH_APP, hInstance, NULL);

    CreateWindow(L"BUTTON", L"File Explr",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX + spacingX * 3, startY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_OPEN_FILE, hInstance, NULL);

    CreateWindow(L"BUTTON", L"Button 5",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX, startY + spacingY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_5, hInstance, NULL);

    CreateWindow(L"BUTTON", L"Button 6",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX + spacingX, startY + spacingY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_6, hInstance, NULL);

    CreateWindow(L"BUTTON", L"Button 7",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX + spacingX * 2, startY + spacingY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_7, hInstance, NULL);

    CreateWindow(L"BUTTON", L"Button 8",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        startX + spacingX * 3, startY + spacingY, btnWidth, btnHeight,
        hwnd, (HMENU)BTN_8, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case BTN_RES_1920:
            ChangeResolution(1920, 1080);
            break;
        case BTN_RES_1280:
            ChangeResolution(1720, 1080);
            break;
        case BTN_LAUNCH_APP:
            LaunchApp(L"notepad.exe");
            break;
        case BTN_OPEN_FILE:
            OpenFile(L"C:\\");
            break;
        case BTN_5:
            CustomAction(L"Button 5");
            break;
        case BTN_6:
            CustomAction(L"Button 6");
            break;
        case BTN_7:
            CustomAction(L"Button 7");
            break;
        case BTN_8:
            CustomAction(L"Button 8");
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ChangeResolution(int width, int height) {
    DEVMODE devMode;
    ZeroMemory(&devMode, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);
    devMode.dmPelsWidth = width;
    devMode.dmPelsHeight = height;
    devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

    LONG result = ChangeDisplaySettings(&devMode, 0);

    if (result == DISP_CHANGE_SUCCESSFUL) {
        std::wstring msg = L"Resolution changed to " + std::to_wstring(width) + L"x" + std::to_wstring(height);
        MessageBox(NULL, msg.c_str(), L"Success", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(NULL, L"Failed to change resolution", L"Error", MB_OK | MB_ICONERROR);
    }
}

void LaunchApp(const wchar_t* appPath) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wchar_t cmdLine[MAX_PATH];
    wcscpy_s(cmdLine, appPath);

    if (CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        MessageBox(NULL, L"Application launched", L"Success", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(NULL, L"Failed to launch application", L"Error", MB_OK | MB_ICONERROR);
    }
}

void OpenFile(const wchar_t* filePath) {
    HINSTANCE result = ShellExecute(NULL, L"open", filePath, NULL, NULL, SW_SHOWDEFAULT);

    if ((int)result > 32) {
        MessageBox(NULL, L"File/folder opened", L"Success", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(NULL, L"Failed to open file/folder", L"Error", MB_OK | MB_ICONERROR);
    }
}

void CustomAction(const wchar_t* buttonName) {
    std::wstring msg = buttonName;
    msg += L" clicked - add your custom action here!";
    MessageBox(NULL, msg.c_str(), L"Action", MB_OK | MB_ICONINFORMATION);
}