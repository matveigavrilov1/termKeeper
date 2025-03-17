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

void inputm::worker()
{
	while (running_)
	{
		if (_kbhit())
		{
			char ch = _getch();

			if (ch == '\r')
			{
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
				{
					LOG_DBG("Shift + Enter pressed");
					pushInputEvent(inputEvent::SHIFT_ENTER);
				}
				else
				{
					LOG_DBG("Enter pressed");
					pushInputEvent(inputEvent::ENTER);
				}
			}
			else if (ch == '\b')
			{
				LOG_DBG("Backspace pressed");
				pushInputEvent(inputEvent::BACKSPACE);
			}
			else if (ch == 27)
			{
				LOG_DBG("Escape pressed");
				pushInputEvent(inputEvent::ESC);
			}
			else if (ch == -32 || ch == 0xE0)
			{
				char specialKey = _getch();
				switch (specialKey)
				{
					case 75:
						LOG_DBG("Left arrow pressed");
						pushInputEvent(inputEvent::ARROW_LEFT);
						break;
					case 77:
						LOG_DBG("Right arrow pressed");
						pushInputEvent(inputEvent::ARROW_RIGHT);
						break;
					case 72:
						LOG_DBG("Up arrow pressed");
						pushInputEvent(inputEvent::ARROW_UP);
						break;
					case 80:
						LOG_DBG("Down arrow pressed");
						pushInputEvent(inputEvent::ARROW_DOWN);
						break;
				}
			}
			else if (ch == 3) // Ctrl+C
			{
				LOG_DBG("Ctrl+C pressed");
				pushInputEvent(inputEvent::CTRL_C);
			}
			else if (ch >= 32 && ch <= 126)
			{
				LOG_DBG("Key pressed: " << ch);
				pushInputEvent(inputEvent::KEY_PRESSED, ch);
			}
			else
			{
				LOG_DBG("Unknown key pressed");
				pushInputEvent(inputEvent::UNKNOWN, ch);
			}
		}
	}
}
} // namespace tk