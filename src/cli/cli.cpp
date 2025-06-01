#include "cli/cli.h"

#include "cli/core/events.h"
#include "cli/core/window.h"
#include "config/config.h"

#include "cli/core/interface.h"
#include "cli/core/utils.h"

#include "cli/windows/cacheWindow.h"
#include "cli/windows/menuWindow.h"
#include "cli/windows/storageWindow.h"

#include "utils/logger.h"

using core = tk::cli::core;

namespace tk
{

cliImpl::cliImpl(cache::shared_ptr_t cache, storage::shared_ptr_type storage)
: cache_(cache)
, storage_(storage)
{
	LOG_DBG("Creating Menu Window");
	auto menuWindow = std::make_shared<tk::menuWindow>();
	menuWindow->setRelativeSize({ 0.2, 1 });
	menuWindow->setPosition({ 0, 0 });
	menuWindow->clear();
	LOG_DBG("Creating Storage Window");
	auto storageWindow = std::make_shared<tk::storageWindow>(storage, cache);
	storageWindow->setRelativeSize({ 0.8, 1 });
	storageWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	storageWindow->clear();
	LOG_DBG("Creating Cache Window");
	auto cacheWindow = std::make_shared<tk::cacheWindow>(cache);
	cacheWindow->setRelativeSize({ 0.8, 1 });
	cacheWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	cacheWindow->clear();

	cache->attach(cacheWindow);

	LOG_DBG("Adding window: " << menuWindow->name());
	windows_[menuWindow->name()] = menuWindow;
	LOG_DBG("Adding window: " << storageWindow->name());
	windows_[storageWindow->name()] = storageWindow;
	LOG_DBG("Adding window: " << cacheWindow->name());
	windows_[cacheWindow->name()] = cacheWindow;
}

void cliImpl::init()
{
	LOG_INF("Registrating windows:");

	for (const auto& windowName : config::instance().registrated())
	{
		if (windows_.contains(windowName))
		{
			LOG_INF(windowName);
			core::getScreen().registerWindow(windows_[windowName]);
		}
		else
		{
			LOG_WRN("Unknown window name: " << windowName);
		}
	}

	LOG_INF("Adding windows to menu:");
	for (const auto& windowName : config::instance().menu())
	{
		if (windows_["Menu"] && windows_.contains(windowName))
		{
			LOG_INF(windowName);
			static_pointer_cast<menuWindow>(windows_["Menu"])->addWindow(windows_[windowName]);
		}
		else
		{
			LOG_WRN("Unknown window name: " << windowName);
		}
	}

	LOG_INF("Activating windows:");
	for (const auto& windowName : config::instance().activated())
	{
		LOG_INF(windowName);
		auto it = windows_.find(windowName);
		if (it != windows_.end() && it->second)
			core::getScreen().activateWindow(it->second->uuid());
	}

	LOG_INF("Setting controller: " << config::instance().initialController());
	auto it = windows_.find(config::instance().initialController());
	if (it != windows_.end() && it->second)
		core::getScreen().changeControllerWindow(it->second->uuid());

	LOG_INF("Updating all windows:");
	for (auto [_, window] : windows_)
	{
		LOG_INF(window->name());
		window->update();
	}
}

int cliImpl::run()
{
	core::getScreen().show(os::console::get());
	tk::pushInputEvent(inputEvent::UNSPECIFIED);
	return core::getEventManager().run();
}
} // namespace tk