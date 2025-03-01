
#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/windows/cacheWindow.h"
#include "conio.h"
#include <thread>
#include "utils/logger.h"

int main(int, char**)
{
	tk::cli::core::init();

	auto cacheWindow =
		std::make_shared<tk::cacheWindow>(0, 0, tk::cli::core::getConsoleManager().width(), tk::cli::core::getConsoleManager().height());
	tk::cli::core::getScreen().registerWindow("cache", cacheWindow);

	tk::cli::core::getScreen().activateWindow("cache");
	tk::cli::core::getScreen().changeActiveWindow("cache");

	std::thread([]() { tk::cli::core::getEventManager().run(); }).detach();

	cacheWindow->update();
	tk::cli::core::getScreen().show(tk::cli::core::getConsoleManager());
	for (;;)
	{
		if (_kbhit())
		{
			char ch = _getch();
			LOG_INF("Key pressed: " << ch);
			tk::utils::pushInputEvent<char>(ch);
		}
	}
	return 0;
}