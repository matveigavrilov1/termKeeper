#include <thread>

#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/windows/cacheWindow.h"
#include "cli/windows/menuWindow.h"
#include "cli/windows/storageWindow.h"

#include "clipboardController/interface.h"
#include "clipboardController/windowsClipboardController.h"

#include "storage/xmlStorageManager.h"

#include "utils/finally.h"

int main(int, char**)
{
	tk::clipboardController::shared_ptr_type clc = std::make_shared<tk::windowsClipboardController>();
	tk::xmlStorageManager xmlStorage;
	xmlStorage.parse("storage.xml");
	tk::finally xmlStorageDump([&xmlStorage]() { xmlStorage.dump("storage.xml"); });

	auto storage = xmlStorage.getStorage();

	tk::cli::core::init();
	auto menuWindow = std::make_shared<tk::menuWindow>(0, 0, tk::cli::core::getConsoleManager().width(), 3);
	auto storageWindow =
		std::make_shared<tk::storageWindow>(storage, clc, 0, 3, tk::cli::core::getConsoleManager().width(), tk::cli::core::getConsoleManager().height() - 3);
	auto cacheWindow = std::make_shared<tk::cacheWindow>(0, 3, tk::cli::core::getConsoleManager().width(), tk::cli::core::getConsoleManager().height() - 3);

	tk::cli::core::getScreen().registerWindow(menuWindow);
	tk::cli::core::getScreen().registerWindow(storageWindow);
	tk::cli::core::getScreen().registerWindow(cacheWindow);

	menuWindow->addWindow(storageWindow);
	menuWindow->addWindow(cacheWindow);

	tk::cli::core::getScreen().activateWindow(menuWindow->name());
	tk::cli::core::getScreen().activateWindow(storageWindow->name());

	tk::cli::core::getScreen().changeControllerWindow(menuWindow->name());
	tk::pushInputEvent(tk::inputEvent::UNSPECIFIED);
	auto eventManagerThread = std::thread([]() { tk::cli::core::getEventManager().run(); });

	menuWindow->update();
	storageWindow->update();
	cacheWindow->update();

	tk::cli::core::getScreen().show(tk::cli::core::getConsoleManager());

	tk::cli::core::getInputManager().run();

	eventManagerThread.join();

	return 0;
}