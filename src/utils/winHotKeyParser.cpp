


#include "utils/winHotKeyParser.h"

#include <algorithm>
#include <map>

namespace tk
{
bool parseHotkey(const std::string& hotkeyStr, UINT& modifiers, UINT& vk)
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
} // namespace tk