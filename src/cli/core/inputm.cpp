#include "cli/core/inputm.h"

#include <conio.h>

#include "cli/core/events.h"
#include "cli/core/utils.h"
#include "utils/logger.h"

namespace tk
{

void inputm::run()
{
	if (running_)
	{
		return;
	}
	running_ = true;

	worker();
}

void inputm::stop()
{
	running_ = false;
}

int readInput()
{
	return std::cin.get();
}

void inputm::worker()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
	{
		std::cerr << "GetStdHandle failed (" << GetLastError() << ")\n";
		running_ = false; // Stop the thread if we can't get the input handle.
		return;
	}

	INPUT_RECORD ir[128];
	DWORD cNumRead;

	while (running_)
	{
		if (!ReadConsoleInput(hStdin, ir, 128, &cNumRead))
		{
			continue; // Or break if you want to stop on error
		}

		for (DWORD i = 0; i < cNumRead; i++)
		{
			if (ir[i].EventType == KEY_EVENT)
			{
				KEY_EVENT_RECORD& ker = ir[i].Event.KeyEvent;
				if (ker.bKeyDown)
				{
					inputEvent::type eventType = inputEvent::UNSPECIFIED;
					char rawChar = ker.uChar.AsciiChar;
					WORD virtualKeyCode = ker.wVirtualKeyCode;

					if (virtualKeyCode == VK_RETURN)
					{
						if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
						{
							eventType = inputEvent::SHIFT_ENTER;
						}
						else
						{
							eventType = inputEvent::ENTER;
						}
					}
					else if (virtualKeyCode == VK_BACK)
					{
						eventType = inputEvent::BACKSPACE;
					}
					else if (virtualKeyCode == VK_ESCAPE)
					{
						pushExitEvent();
						continue;
					}
					else if (virtualKeyCode == VK_LEFT)
					{
						eventType = inputEvent::ARROW_LEFT;
					}
					else if (virtualKeyCode == VK_RIGHT)
					{
						eventType = inputEvent::ARROW_RIGHT;
					}
					else if (virtualKeyCode == VK_UP)
					{
						eventType = inputEvent::ARROW_UP;
					}
					else if (virtualKeyCode == VK_DOWN)
					{
						eventType = inputEvent::ARROW_DOWN;
					}
					else if (virtualKeyCode == VK_HOME)
					{
						eventType = inputEvent::HOME;
					}
					else if (virtualKeyCode == VK_END)
					{
						eventType = inputEvent::END;
					}
					else if (virtualKeyCode == VK_PRIOR)
					{
						eventType = inputEvent::PAGE_UP;
					}
					else if (virtualKeyCode == VK_NEXT)
					{
						eventType = inputEvent::PAGE_DOWN;
					}
					else if (virtualKeyCode == VK_INSERT)
					{
						eventType = inputEvent::INSERT;
					}
					else if (virtualKeyCode == VK_DELETE)
					{
						eventType = inputEvent::DELETE_KEY;
					}
					else if (virtualKeyCode == VK_F1)
					{
						eventType = inputEvent::F1;
					}
					else if (virtualKeyCode == VK_F2)
					{
						eventType = inputEvent::F2;
					}
					else if (virtualKeyCode == VK_F3)
					{
						eventType = inputEvent::F3;
					}
					else if (virtualKeyCode == VK_F4)
					{
						eventType = inputEvent::F4;
					}
					else if (virtualKeyCode == VK_F5)
					{
						eventType = inputEvent::F5;
					}
					else if (virtualKeyCode == VK_F6)
					{
						eventType = inputEvent::F6;
					}
					else if (virtualKeyCode == VK_F7)
					{
						eventType = inputEvent::F7;
					}
					else if (virtualKeyCode == VK_F8)
					{
						eventType = inputEvent::F8;
					}
					else if (virtualKeyCode == VK_F9)
					{
						eventType = inputEvent::F9;
					}
					else if (virtualKeyCode == VK_F10)
					{
						eventType = inputEvent::F10;
					}
					else if (virtualKeyCode == VK_F11)
					{
						eventType = inputEvent::F11;
					}
					else if (virtualKeyCode == VK_F12)
					{
						eventType = inputEvent::F12;
					}
					else if (rawChar >= 1 && rawChar <= 26)
					{
						if (rawChar == 1)
						{
							eventType = inputEvent::CTRL_A;
						}
						else if (rawChar == 2)
						{
							eventType = inputEvent::CTRL_B;
						}
						else if (rawChar == 3)
						{
							eventType = inputEvent::CTRL_C;
						}
						else
						{
							LOG_DBG("Unknown Ctrl Key pressed: " << static_cast<int>(rawChar));
						}
					}
					else if (rawChar >= 32 && rawChar <= 126)
					{
						eventType = inputEvent::KEY_PRESSED;
					}
					else
					{
						if (rawChar != 0)
							LOG_DBG("Character " << static_cast<int>(rawChar));
					}

					pushInputEvent(eventType, rawChar);
				}
			}
		}
	}
}

} // namespace tk