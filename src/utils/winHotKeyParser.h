#include <windows.h>
#include <shellapi.h>
#include <string>

namespace utils
{
bool parseHotkey(const std::string& hotkeyStr, UINT& modifiers, UINT& vk);
}