#include "clipboardController/windowsClipboardController.h"

#include <windows.h>
#include <string>
#include <codecvt>
#include <locale> 

#include "utils/logger.h"

void tk::windowsClipboardController::write(const std::string& content)
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