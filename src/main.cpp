#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/windows/cacheWindow.h"
#include "cli/windows/menuWindow.h"
#include <thread>

int main(int, char**)
{
	tk::cli::core::init();
	auto menuWindow = std::make_shared<tk::menuWindow>(0, 0, tk::cli::core::getConsoleManager().width(), 3);
	auto cacheWindow = std::make_shared<tk::cacheWindow>(0, 3, tk::cli::core::getConsoleManager().width(), tk::cli::core::getConsoleManager().height() - 3);
	auto cacheWindow1 = std::make_shared<tk::cacheWindow>(0, 3, tk::cli::core::getConsoleManager().width(), tk::cli::core::getConsoleManager().height() - 3, "Cache1");


	tk::cli::core::getScreen().registerWindow(menuWindow);
	tk::cli::core::getScreen().registerWindow(cacheWindow);
	tk::cli::core::getScreen().registerWindow(cacheWindow1);

	menuWindow->addWindow(cacheWindow);
	menuWindow->addWindow(cacheWindow1);

	tk::cli::core::getScreen().activateWindow(menuWindow->name());
	tk::cli::core::getScreen().activateWindow(cacheWindow->name());
	tk::cli::core::getScreen().activateWindow(cacheWindow1->name());

	tk::cli::core::getScreen().changeControllerWindow(menuWindow->name());
	tk::pushInputEvent(tk::inputEvent::UNKNOWN);
	auto eventManagerThread = std::thread([]() { tk::cli::core::getEventManager().run(); });
	
	menuWindow->update();
	cacheWindow->update();
	cacheWindow1->update();

	tk::cli::core::getScreen().show(tk::cli::core::getConsoleManager());

	tk::cli::core::getInputManager().run();

	eventManagerThread.join();

	return 0;
}