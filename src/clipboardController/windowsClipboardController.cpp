#include "clipboardController/windowsClipboardController.h"

#include <windows.h>

#include "logger.h"

void tk::windowsClipboardController::write(const std::string& content)
{
	if (!OpenClipboard(nullptr))
	{
		LOG_ERR("OpenClipboard failed: " << GetLastError());
		return;
	}
	EmptyClipboard();
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (content.size() + 1) * sizeof(char));
	if (hGlobal == nullptr)
	{
		LOG_ERR("GlobalAlloc failed: " << GetLastError());
		CloseClipboard();
		return;
	}
	char* pGlobal = static_cast<char*>(GlobalLock(hGlobal));
	if (pGlobal == nullptr)
	{
		LOG_ERR("GlobalLock failed: " << GetLastError());
		CloseClipboard();
		GlobalFree(hGlobal);
		return;
	}
	strcpy_s(pGlobal, content.size() + 1, content.c_str());
	GlobalUnlock(hGlobal);
	if (SetClipboardData(CF_UNICODETEXT, hGlobal) == nullptr)
	{
		LOG_ERR("SetClipboardData failed: " << GetLastError());
	}
	CloseClipboard();
}