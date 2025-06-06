#include "os/interface.h"

#include <stdexcept>
#include <windows.h>
#include <string>
#include <codecvt>
#include <locale>
#include <map>

#include "core/utils.h"
#include "config/config.h"
#include "utils/logger.h"
#include "utils/visibilityManager.h"

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAYICON 1
#define HOTKEY_ID 1

namespace
{
HWND hWnd = nullptr;
NOTIFYICONDATA nid = {};
bool trayIconAdded = false;
bool hotkeyRegistered = false;

const char* trayAppName = "unknown";
bool visible = false;
} // namespace

static bool parseHotkey(const std::string& hotkeyStr, UINT& modifiers, UINT& vk)
{
	modifiers = 0;
	vk = 0;

	std::map<std::string, UINT> modMap = { { "Ctrl", MOD_CONTROL }, { "Alt", MOD_ALT }, { "Shift", MOD_SHIFT }, { "Win", MOD_WIN } };

	std::string keyStr;
	size_t pos = 0;
	size_t nextPos;

	while ((nextPos = hotkeyStr.find('+', pos)) != std::string::npos)
	{
		std::string modStr = hotkeyStr.substr(pos, nextPos - pos);
		std::transform(modStr.begin(), modStr.end(), modStr.begin(), ::tolower);
		modStr[0] = toupper(modStr[0]);

		if (modMap.find(modStr) != modMap.end())
		{
			modifiers |= modMap[modStr];
		}
		else
		{
			return false;
		}

		pos = nextPos + 1;
	}

	keyStr = hotkeyStr.substr(pos);
	if (keyStr.length() == 1)
	{
		vk = toupper(keyStr[0]);
	}
	else
	{
		std::transform(keyStr.begin(), keyStr.end(), keyStr.begin(), ::tolower);
		keyStr[0] = toupper(keyStr[0]);

		if (keyStr == "Enter")
		{
			vk = VK_RETURN;
		}
		else if (keyStr == "Space")
		{
			vk = VK_SPACE;
		}
		else if (keyStr == "Escape")
		{
			vk = VK_ESCAPE;
		}
		else if (keyStr == "Tab")
		{
			vk = VK_TAB;
		}
		else if (keyStr == "Backspace")
		{
			vk = VK_BACK;
		}
		else if (keyStr == "Delete")
		{
			vk = VK_DELETE;
		}
		else if (keyStr == "Insert")
		{
			vk = VK_INSERT;
		}
		else if (keyStr == "Home")
		{
			vk = VK_HOME;
		}
		else if (keyStr == "End")
		{
			vk = VK_END;
		}
		else if (keyStr == "Pageup")
		{
			vk = VK_PRIOR;
		}
		else if (keyStr == "Pagedown")
		{
			vk = VK_NEXT;
		}
		else if (keyStr == "Left")
		{
			vk = VK_LEFT;
		}
		else if (keyStr == "Right")
		{
			vk = VK_RIGHT;
		}
		else if (keyStr == "Up")
		{
			vk = VK_UP;
		}
		else if (keyStr == "Down")
		{
			vk = VK_DOWN;
		}
		else if (keyStr.size() > 1 && keyStr[0] == 'F')
		{
			int fNum = std::stoi(keyStr.substr(1));
			if (fNum >= 1 && fNum <= 24)
			{
				vk = VK_F1 + (fNum - 1);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

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

			trayIconAdded = Shell_NotifyIcon(NIM_ADD, &nid);
			if (!trayIconAdded)
			{
				LOG_ERR("Failed to add tray icon: " << GetLastError());
			}

			UINT modifiers;
			UINT vk;
			if (!parseHotkey(conf::config::instance().hotKey(), modifiers, vk))
			{
				throw std::runtime_error("Failed to parse hotkey");
			}
			hotkeyRegistered = RegisterHotKey(hWnd, HOTKEY_ID, modifiers, vk);
			if (!hotkeyRegistered)
			{
				LOG_ERR("Failed to register hotkey: " << GetLastError());
			}
		}
		break;

		case WM_TRAYICON:
			if (lParam == WM_RBUTTONDOWN)
			{
				POINT pt;
				GetCursorPos(&pt);
				HMENU hMenu = CreatePopupMenu();
				if (hMenu)
				{
					AppendMenu(hMenu, MF_STRING, 2, TEXT("Exit"));
					SetForegroundWindow(hWnd);
					TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
					DestroyMenu(hMenu);
				}
			}
			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == 2)
			{
				DestroyWindow(hWnd);
			}
			break;

		case WM_HOTKEY:
			if (wParam == HOTKEY_ID)
			{
				auto console = os::console::get();
				if (console->visible())
				{
					utils::visibilityManager::instance().hide();
				}
				else
				{
					console->show();
				}
			}
			break;

		case WM_DESTROY:
			if (trayIconAdded)
			{
				Shell_NotifyIcon(NIM_DELETE, &nid);
			}
			if (hotkeyRegistered)
			{
				UnregisterHotKey(hWnd, HOTKEY_ID);
			}
			core::pushExitEvent();
			PostQuitMessage(0);
			break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int os::runApp(const char* appName)
{
	SetConsoleOutputCP(437);
	trayAppName = appName;

	HINSTANCE hInstance = GetModuleHandle(nullptr);
	if (!hInstance)
	{
		LOG_ERR("GetModuleHandle failed: " << GetLastError());
		return 1;
	}

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("AppWindowClass");

	if (!RegisterClass(&wc))
	{
		LOG_ERR("Window class registration failed: " << GetLastError());
		return 1;
	}

	hWnd = CreateWindow(wc.lpszClassName, TEXT("Application"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		LOG_ERR("Window creation failed: " << GetLastError());
		return 1;
	}

	auto console = os::console::get();
	console->setConsoleName(appName);
	console->hide();
	console->setCursorVisibility(false);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Очистка при выходе из цикла сообщений
	if (trayIconAdded)
	{
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}
	if (hotkeyRegistered)
	{
		UnregisterHotKey(hWnd, HOTKEY_ID);
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