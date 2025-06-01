#include "os/interface.h"

#include <windows.h>
#include <string>
#include <codecvt>
#include <locale>

#include "cli/core/events.h"
#include "cli/core/utils.h"
#include "utils/logger.h"

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAYICON 1
#define HOTKEY_ID 1

HWND hWnd;
NOTIFYICONDATA nid;

const char* trayAppName = "unknown";
bool visible = false;

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			nid = { sizeof(nid) };
			nid.hWnd = hWnd;
			nid.uID = ID_TRAYICON;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_TRAYICON;
			nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			lstrcpy(nid.szTip, TEXT(trayAppName));
			Shell_NotifyIcon(NIM_ADD, &nid);

			RegisterHotKey(hWnd, HOTKEY_ID, MOD_CONTROL | MOD_SHIFT, 'C');
		}
		break;
		case WM_TRAYICON:
			if (lParam == WM_RBUTTONDOWN)
			{
				POINT pt;
				GetCursorPos(&pt);
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, 2, TEXT("Exit"));
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
				DestroyMenu(hMenu);
			}
			break;
		case WM_COMMAND:
			if (LOWORD(wParam) == 2)
			{
				Shell_NotifyIcon(NIM_DELETE, &nid);
				PostQuitMessage(0);
			}
			break;
		case WM_HOTKEY:
			if (wParam == HOTKEY_ID)
			{
				auto action = visible ? SW_HIDE : SW_SHOW;
				ShowWindow(GetConsoleWindow(), action);
				if (!visible)
					SetForegroundWindow(hWnd);
	
				visible = !visible;
			}
			break;
		case WM_KEYDOWN:
		{
			bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
			bool ctrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
			bool altPressed = (GetKeyState(VK_MENU) & 0x8000) != 0;

			WORD virtualKeyCode = LOWORD(wParam);
			UINT scanCode = (lParam >> 16) & 0x00FF;
			bool isExtended = (lParam & 0x01000000) != 0;

			// Handle extended keys
			if (isExtended)
			{
				scanCode |= 0xE000;
			}

			BYTE keyboardState[256];
			GetKeyboardState(keyboardState);

			// Handle dead keys and proper character conversion
			WCHAR charBuffer[5] = { 0 }; // Increased buffer for surrogate pairs
			int result = ToUnicode(virtualKeyCode, scanCode, keyboardState, charBuffer, sizeof(charBuffer) / sizeof(charBuffer[0]), 0);

			char asciiChar = 0;
			if (result > 0)
			{
				// Convert to UTF-8 if needed
				if (charBuffer[0] < 128)
				{
					asciiChar = static_cast<char>(charBuffer[0]);
				}
				else
				{
					// Handle Unicode characters properly
					std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
					std::string utf8 = converter.to_bytes(charBuffer);
					if (!utf8.empty())
					{
						asciiChar = utf8[0]; // Simplified - consider handling full UTF-8
					}
				}
			}

			LOG_DBG("Key pressed: " << (int)virtualKeyCode << " Char: " << (int)asciiChar);

			tk::inputEvent::type eventType = tk::inputEvent::UNSPECIFIED;

			// Handle special keys
			switch (virtualKeyCode)
			{
				case VK_RETURN: eventType = tk::inputEvent::ENTER; break;
				case VK_ESCAPE: eventType = tk::inputEvent::ESC; break;
				case VK_HOME: eventType = tk::inputEvent::HOME; break;
				case VK_END: eventType = tk::inputEvent::END; break;
				case VK_PRIOR: eventType = tk::inputEvent::PAGE_UP; break;
				case VK_NEXT: eventType = tk::inputEvent::PAGE_DOWN; break;
				case VK_INSERT: eventType = tk::inputEvent::INSERT; break;
				case VK_DELETE: eventType = tk::inputEvent::DELETE_KEY; break;
				case VK_LEFT: eventType = tk::inputEvent::ARROW_LEFT; break;
				case VK_RIGHT: eventType = tk::inputEvent::ARROW_RIGHT; break;
				case VK_UP: eventType = tk::inputEvent::ARROW_UP; break;
				case VK_DOWN: eventType = tk::inputEvent::ARROW_DOWN; break;
				case VK_F1: eventType = tk::inputEvent::F1; break;
				case VK_F2: eventType = tk::inputEvent::F2; break;
				case VK_F3: eventType = tk::inputEvent::F3; break;
				case VK_F4: eventType = tk::inputEvent::F4; break;
				case VK_F5: eventType = tk::inputEvent::F5; break;
				case VK_F6: eventType = tk::inputEvent::F6; break;
				case VK_F7: eventType = tk::inputEvent::F7; break;
				case VK_F8: eventType = tk::inputEvent::F8; break;
				case VK_F9: eventType = tk::inputEvent::F9; break;
				case VK_F10: eventType = tk::inputEvent::F10; break;
				case VK_F11: eventType = tk::inputEvent::F11; break;
				case VK_F12: eventType = tk::inputEvent::F12; break;
				case VK_BACK: eventType = tk::inputEvent::BACKSPACE; break;
				default: eventType = tk::inputEvent::KEY_PRESSED; break;
			}

			tk::pushInputEvent(eventType, asciiChar, shiftPressed, ctrlPressed, altPressed);
		}
		break;
		case WM_DESTROY:
		{
			Shell_NotifyIcon(NIM_DELETE, &nid);
			PostQuitMessage(0);
		}
		break;

		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int os::runApp(const char* appName, std::atomic<bool>& running)
{
	SetConsoleOutputCP(437);
	trayAppName = appName;

	HINSTANCE hInstance = GetModuleHandle(nullptr);
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT(appName);
	RegisterClass(&wc);

	hWnd = CreateWindow(wc.lpszClassName, TEXT(appName),
		WS_OVERLAPPEDWINDOW,					// Добавлен стиль окна
		CW_USEDEFAULT, CW_USEDEFAULT, // Позиция
		400, 300,											// Размеры
		NULL, NULL, hInstance, NULL);
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void os::writeToClipboard(const std::string& content)
{
	if (!OpenClipboard(nullptr))
	{
		LOG_ERR("OpenClipboard failed: " << GetLastError());
		return;
	}
	EmptyClipboard();

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wcontent = converter.from_bytes(content);

	size_t bufferSize = (wcontent.size() + 1) * sizeof(wchar_t);

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, bufferSize);
	if (hGlobal == nullptr)
	{
		LOG_ERR("GlobalAlloc failed: " << GetLastError());
		CloseClipboard();
		return;
	}

	wchar_t* pGlobal = static_cast<wchar_t*>(GlobalLock(hGlobal));
	if (pGlobal == nullptr)
	{
		LOG_ERR("GlobalLock failed: " << GetLastError());
		CloseClipboard();
		GlobalFree(hGlobal);
		return;
	}

	wcscpy_s(pGlobal, wcontent.size() + 1, wcontent.c_str());

	GlobalUnlock(hGlobal);

	if (SetClipboardData(CF_UNICODETEXT, hGlobal) == nullptr)
	{
		LOG_ERR("SetClipboardData failed: " << GetLastError());
		GlobalFree(hGlobal);
	}

	CloseClipboard();
}