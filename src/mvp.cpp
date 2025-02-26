#include <windows.h>
#include <vector>
#include <string>
#include <iostream> // Добавлено для отладочного вывода

// Глобальные переменные
std::vector<std::wstring> commands = {
    L"git clone",
    L"docker build -t myimage .",
    L"npm install",
    L"python3 -m venv venv",
    L"kubectl apply -f deployment.yaml"
};
HWND hListBox;
HWND hwndMain; // Добавлено для хранения дескриптора главного окна

// Функция для копирования текста в буфер обмена
void CopyToClipboard(const std::wstring& text) {
    if (!OpenClipboard(nullptr)) {
        std::wcerr << L"OpenClipboard failed: " << GetLastError() << std::endl;
        return;
    }
    EmptyClipboard();
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
    if (hGlobal == nullptr) {
        std::wcerr << L"GlobalAlloc failed: " << GetLastError() << std::endl;
        CloseClipboard();
        return;
    }
    wchar_t* pGlobal = static_cast<wchar_t*>(GlobalLock(hGlobal));
    if (pGlobal == nullptr) {
        std::wcerr << L"GlobalLock failed: " << GetLastError() << std::endl;
        CloseClipboard();
        GlobalFree(hGlobal);
        return;
    }
    wcscpy_s(pGlobal, text.size() + 1, text.c_str());
    GlobalUnlock(hGlobal);
    if (SetClipboardData(CF_UNICODETEXT, hGlobal) == nullptr) {
        std::wcerr << L"SetClipboardData failed: " << GetLastError() << std::endl;
    }
    CloseClipboard();
}

// Обработчик сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Создаем список команд
            hListBox = CreateWindowW(
                L"LISTBOX", L"Commands",
                WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
                10, 10, 300, 200, hwnd, (HMENU)1, nullptr, nullptr  // Установили ID контрола в 1
            );
            for (const auto& cmd : commands) {
                SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)cmd.c_str());
            }
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 1 && HIWORD(wParam) == LBN_DBLCLK) {
                // Получаем выбранную команду
                int index = static_cast<int>(SendMessageW(hListBox, LB_GETCURSEL, 0, 0));
                if (index != LB_ERR) {
                    wchar_t buffer[256];
                    SendMessageW(hListBox, LB_GETTEXT, index, (LPARAM)buffer);
                    CopyToClipboard(buffer);
// Скрываем окно, а не закрываем его
                    ShowWindow(hwnd, SW_HIDE);
                }
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_ACTIVATE:
            // При активации окна (например, после нажатия горячей клавиши),
            // устанавливаем фокус на список, чтобы можно было использовать стрелки для выбора.
            if (wParam != WA_INACTIVE) {
                SetFocus(hListBox);
            }
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Регистрация горячей клавиши
void RegisterHotKey(HWND hwnd) {
    if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT, 'C')) {
        std::wcerr << L"RegisterHotKey failed: " << GetLastError() << std::endl;
    }
}

// Точка входа
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ClipboardToolClass";
    RegisterClassW(&wc);

    // Создание окна
    hwndMain = CreateWindowExW(
        0, L"ClipboardToolClass", L"Clipboard Tool",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 300,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hwndMain) {
        std::wcerr << L"CreateWindowExW failed: " << GetLastError() << std::endl;
        return -1;
    }

    // Регистрация горячей клавиши
    RegisterHotKey(hwndMain);

    // Изначально скрываем окно
    ShowWindow(hwndMain, SW_HIDE);
    UpdateWindow(hwndMain);

    // Цикл сообщений
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            // Показываем окно при нажатии горячей клавиши
            ShowWindow(hwndMain, SW_SHOW);
            UpdateWindow(hwndMain);

             // Устанавливаем окно на передний план
            SetForegroundWindow(hwndMain);
            SetFocus(hListBox); // Устанавливаем фокус на hListBox
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}