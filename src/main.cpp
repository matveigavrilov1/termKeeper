#include "core/interface.h"
#include "ui/ui.h"

#include "config/config.h"

#include "os/console.h"

#include "os/interface.h"
#include "data/xmlStorageManager.h"
#include "data/xmlCacheManager.h"

#include "utils/finally.h"
#include "utils/logger.h"
#include "utils/visibilityManager.h"

#include <cstdlib>

int main(int, char**)
{
	LOG_INF("Starting project controller");

	conf::config::instance().init();
	LOG_INF("Config initialized");

	data::xmlStorageManager xmlStorage;
	xmlStorage.parse(conf::config::instance().storageFile());
	auto xmlStorageDumpCallback = [&xmlStorage]()
	{
		xmlStorage.dump(conf::config::instance().storageFile());
	};
	utils::visibilityManager::instance().addCallback(xmlStorageDumpCallback);
	utils::finally xmlStorageDump(xmlStorageDumpCallback);
	LOG_INF("XML storage loaded");

	data::xmlCacheManager xmlCache;
	xmlCache.parse(conf::config::instance().cacheFile());
	auto xmlCacheDumpCallback = [&xmlCache]()
	{
		xmlCache.dump(conf::config::instance().cacheFile());
	};
	utils::visibilityManager::instance().addCallback(xmlCacheDumpCallback);
	utils::finally xmlCacheDump(xmlCacheDumpCallback);
	LOG_INF("XML cache loaded");

	std::thread([&]() { os::runApp("ActionControl"); }).detach();
	LOG_INF("Event loop started");
	auto screenWidth = conf::config::instance().screenWidth();
	auto screenHeight = conf::config::instance().screenHeight();
	if (screenWidth && screenHeight && !os::console::get()->setConsoleSize({ screenWidth, screenHeight }))
	{
		LOG_ERR("Failed to set screen size");
		return EXIT_FAILURE;
	}
	LOG_INF("Window size set");

	auto storage = xmlStorage.getStorage();
	auto cache = xmlCache.getCache();

	core::init();

	ui::uiImpl ui(cache, storage);
	LOG_INF("CLI created");
	ui.init();

	return ui.run();
}