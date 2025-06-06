#include "ui/ui.h"

#include "core/controllerm.h"
#include "core/events.h"
#include "core/window.h"
#include "config/config.h"

#include "core/interface.h"
#include "core/utils.h"

#include "windows/cacheWindow.h"
#include "windows/menuWindow.h"
#include "windows/storageWindow.h"

#include "utils/logger.h"
#include <memory>

namespace ui
{

uiImpl::uiImpl(data::cache::shared_ptr_t cache, data::storage::shared_ptr_t storage)
: cache_(cache)
, storage_(storage)
{
	LOG_DBG("Creating Menu Window");
	auto menuWindow = std::make_shared<wndws::menuWindow>();
	menuWindow->setRelativeSize({ 0.1, 1 });
	menuWindow->setPosition({ 0, 0 });
	menuWindow->clear();
	LOG_DBG("Creating Storage Window");
	auto storageWindow = std::make_shared<wndws::storageWindow>(storage, cache);
	storageWindow->setRelativeSize({ 0.9, 1 });
	storageWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	storageWindow->clear();
	storageWindow->setRelativePos({.left = menuWindow->uuid()});
	LOG_DBG("Creating Cache Window");
	auto cacheWindow = std::make_shared<wndws::cacheWindow>(cache);
	cacheWindow->setRelativeSize({ 0.9, 1 });
	cacheWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	cacheWindow->clear();
	cacheWindow->setRelativePos({.left = menuWindow->uuid()});

	cache->attach(cacheWindow);

	LOG_DBG("Adding window: " << menuWindow->name());
	windows_[menuWindow->name()] = menuWindow;
	LOG_DBG("Adding window: " << storageWindow->name());
	windows_[storageWindow->name()] = storageWindow;
	LOG_DBG("Adding window: " << cacheWindow->name());
	windows_[cacheWindow->name()] = cacheWindow;
}

void uiImpl::init()
{
	LOG_INF("Registrating windows:");

	for (const auto& windowName : conf::config::instance().registrated())
	{
		if (windows_.contains(windowName))
		{
			LOG_INF(windowName);
			core::screen().registerWindow(windows_[windowName]);
			auto controller = std::dynamic_pointer_cast<wndws::controllerWindow>(windows_[windowName]);
			if (controller)
				core::controllerm().registerController(controller);
		}
		else
		{
			LOG_WRN("Unknown window name: " << windowName);
		}
	}

	LOG_INF("Adding windows to menu:");
	for (const auto& windowName : conf::config::instance().menu())
	{
		if (windows_["Menu"] && windows_.contains(windowName))
		{
			LOG_INF(windowName);
			static_pointer_cast<wndws::menuWindow>(windows_["Menu"])->addWindow(windows_[windowName]);
		}
		else
		{
			LOG_WRN("Unknown window name: " << windowName);
		}
	}

	LOG_INF("Activating windows:");
	for (const auto& windowName : conf::config::instance().activated())
	{
		LOG_INF(windowName);
		auto it = windows_.find(windowName);
		if (it != windows_.end() && it->second)
			core::screen().activateWindow(it->second->uuid());
	}

	LOG_INF("Setting controller: " << conf::config::instance().initialController());
	auto it = windows_.find(conf::config::instance().initialController());
	if (it != windows_.end() && it->second)
		core::controllerm().setActiveController(it->second->uuid());

	LOG_INF("Updating all windows:");
	for (auto [_, window] : windows_)
	{
		LOG_INF(window->name());
		window->update();
	}
}

int uiImpl::run()
{
	core::screen().show(os::console::get());
	core::pushInputEvent(core::inputEvent::UNSPECIFIED);
	return core::eventm().run();
}
} // namespace ui