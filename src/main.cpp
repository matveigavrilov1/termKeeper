#include "cli/core/interface.h"
#include "cli/windows/cacheWindow.h"
#include <thread>

int main(int, char**)
{
	tk::cli::core::init();

	auto cacheWindow = std::make_shared<tk::cacheWindow>(0, 0, tk::cli::core::getConsoleManager().width(), tk::cli::core::getConsoleManager().height());
	tk::cli::core::getScreen().registerWindow("cache", cacheWindow);

	tk::cli::core::getScreen().activateWindow("cache");
	tk::cli::core::getScreen().changeControllerWindow("cache");

	auto eventManagerThread = std::thread([]() { tk::cli::core::getEventManager().run(); });

	cacheWindow->update();
	tk::cli::core::getScreen().show(tk::cli::core::getConsoleManager());

	tk::cli::core::getInputManager().run();

	eventManagerThread.join();

	return 0;
}