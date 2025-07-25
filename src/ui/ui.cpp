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

#include "windows/searchWindow.h"
#include "windows/actionsWindow.h"
#include "windows/descriptionWindow.h"

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

	menuWindow->addWindow(menuWindow->createGroup("Storage"), storageWindow->uuid());
	menuWindow->addWindow(menuWindow->createGroup("Cache"), cacheWindow->uuid());

	auto searchWindow = std::make_shared<wndws::searchWindow>();
	searchWindow->setRelativeSize({ 0.9, 0.1 });
	searchWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	searchWindow->clear();
	searchWindow->setRelativePos({.left = menuWindow->uuid()});

	auto actionsWindow = std::make_shared<wndws::actionsWindow>();
	actionsWindow->setRelativeSize({ 0.9, 0.5 });
	actionsWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	actionsWindow->clear();
	actionsWindow->setRelativePos({.left = menuWindow->uuid(), .upper = searchWindow->uuid()});

	auto descriptionWindow = std::make_shared<wndws::descriptionWindow>();
	descriptionWindow->setRelativeSize({ 0.9, 0.4 });
	descriptionWindow->setPosition({ menuWindow->realPos().x + menuWindow->realWidth(), 0 });
	descriptionWindow->clear();
	descriptionWindow->setRelativePos({.left = menuWindow->uuid(), .upper = actionsWindow->uuid()});

	auto uuid = menuWindow->createGroup("Actions");
	menuWindow->addWindow(uuid, searchWindow->uuid());
	menuWindow->addWindow(uuid, actionsWindow->uuid());
	menuWindow->addWindow(uuid, descriptionWindow->uuid());
	

	LOG_DBG("Adding window: " << menuWindow->name());
	windows_[menuWindow->name()] = menuWindow;
	LOG_DBG("Adding window: " << storageWindow->name());
	windows_[storageWindow->name()] = storageWindow;
	LOG_DBG("Adding window: " << cacheWindow->name());
	windows_[cacheWindow->name()] = cacheWindow; 
	
	LOG_DBG("Adding window: " << searchWindow->name());
	windows_[searchWindow->name()] = searchWindow;
	LOG_DBG("Adding window: " << actionsWindow->name());
	windows_[actionsWindow->name()] = actionsWindow;
	LOG_DBG("Adding window: " << descriptionWindow->name());
	windows_[descriptionWindow->name()] = descriptionWindow;
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