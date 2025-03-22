#include "cli/cli.h"

#include <thread>

#include "cli/core/events.h"
#include "config/config.h"

#include "cli/core/interface.h"
#include "cli/core/utils.h"

#include "cli/windows/cacheWindow.h"
#include "cli/windows/menuWindow.h"
#include "cli/windows/storageWindow.h"

using core = tk::cli::core;

namespace tk
{

cliImpl::cliImpl(clipboardController::shared_ptr_type clc, cache::shared_ptr_type cache, storage::shared_ptr_type storage)
: clc_(clc)
, cache_(cache)
, storage_(storage)
{
	auto menuWindow = std::make_shared<tk::menuWindow>(0, 0, core::getConsoleManager().width(), 3);
	auto storageWindow =
		std::make_shared<tk::storageWindow>(storage, clc, cache, 0, 3, core::getConsoleManager().width(), core::getConsoleManager().height() - 3);
	auto cacheWindow = std::make_shared<tk::cacheWindow>(cache, clc, 0, 3, core::getConsoleManager().width(), core::getConsoleManager().height() - 3);

	cache->attach(cacheWindow);

	windows_[menuWindow->name()] = menuWindow;
	windows_[storageWindow->name()] = storageWindow;
	windows_[cacheWindow->name()] = cacheWindow;
}

void cliImpl::init()
{
	for (const auto& windowName : config::instance().registrated())
	{
		if (windows_.contains(windowName))
			core::getScreen().registerWindow(windows_[windowName]);
	}

	for (const auto& windowName : config::instance().menu())
	{
		if (windows_["Menu"] && windows_.contains(windowName))
		{
			static_pointer_cast<menuWindow>(windows_["Menu"])->addWindow(windows_[windowName]);
		}
	}

	for (const auto& windowName : config::instance().activated())
	{
		core::getScreen().activateWindow(windowName);
	}

	core::getScreen().changeControllerWindow(config::instance().initialController());

	for (auto [_, window] : windows_)
	{
		window->update();
	}
}

int cliImpl::run()
{
	core::getScreen().show(tk::cli::core::getConsoleManager());

	auto eventManagerThread = std::jthread([]() { core::getEventManager().run(); });
	tk::pushInputEvent(inputEvent::UNSPECIFIED);

	core::getInputManager().run();

	return 0;
}
} // namespace tk