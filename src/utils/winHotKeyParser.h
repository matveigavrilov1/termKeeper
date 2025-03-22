#include <windows.h>
#include <shellapi.h>
#include <string>

namespace tk
{
bool parseHotkey(const std::string& hotkeyStr, UINT& modifiers, UINT& vk);
}