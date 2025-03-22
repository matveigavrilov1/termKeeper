#include <stdexcept>
#include <windows.h>
#include <shellapi.h>

#include "config/config.h"
#include "utils/winHotKeyParser.h"

#define HOTKEY_ID 1
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_RUN 1002

NOTIFYICONDATA nid;
HMENU hMenu;
HINSTANCE hInst;

void ShowContextMenu(HWND hwnd)
{
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow(hwnd);
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
	PostMessage(hwnd, WM_NULL, 0, 0);
}

void AddTrayIcon(HWND hwnd)
{
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(101));
	lstrcpy(nid.szTip, "TermKeeper");
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon(HWND hwnd)
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void RunExe()
{
	ShellExecute(NULL, "open", tk::config::instance().executable().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, ID_TRAY_RUN, "Run");
			AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, "Exit");
			UINT modifiers, vk;
			if (tk::parseHotkey(tk::config::instance().hotKey(), modifiers, vk))
			{
				RegisterHotKey(hwnd, HOTKEY_ID, modifiers, vk);
			}
			else
			{
				MessageBox(hwnd, "Invalid hotkey format!", "Error", MB_ICONERROR | MB_OK);
				throw std::runtime_error("Invalid hotkey format");
			}
			AddTrayIcon(hwnd);
			break;

		case WM_TRAYICON:
			if (lParam == WM_RBUTTONUP)
			{
				ShowContextMenu(hwnd);
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_TRAY_RUN: RunExe(); break;
				case ID_TRAY_EXIT: DestroyWindow(hwnd); break;
			}
			break;

		case WM_HOTKEY:
			if (wParam == HOTKEY_ID)
			{
				RunExe();
			}
			break;

		case WM_DESTROY:
			RemoveTrayIcon(hwnd);
			UnregisterHotKey(hwnd, HOTKEY_ID);
			PostQuitMessage(0);
			break;

		default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	tk::config::instance().init();

	hInst = hInstance;

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "TrayAppClass";

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(0, "TrayAppClass", "Tray App", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}