#include "core/interface.h"
#include "ui/ui.h"

#include "config/config.h"

#include "os/console.h"

#include "os/interface.h"
#include "data/xmlStorageManager.h"
#include "data/xmlCacheManager.h"

#include "utils/finally.h"
#include "utils/logger.h"

#include <cstdlib>

std::atomic<bool> running = true;

int main(int, char**)
{
	LOG_INF("Starting project controller");
	std::thread([&]() { os::runApp("project-controller", running); }).detach();
	LOG_INF("Event loop started");

	conf::config::instance().init();
	LOG_INF("Config initialized");

	auto screenWidth = conf::config::instance().screenWidth();
	auto screenHeight = conf::config::instance().screenHeight();
	if (screenWidth && screenHeight && !os::console::get()->setConsoleSize({ screenWidth, screenHeight }))
	{
		LOG_ERR("Failed to set screen size");
		return EXIT_FAILURE;
	}
	LOG_INF("Window size set");

	data::xmlStorageManager xmlStorage;
	xmlStorage.parse(conf::config::instance().storageFile());
	utils::finally xmlStorageDump([&xmlStorage]() { xmlStorage.dump(conf::config::instance().storageFile()); });
	LOG_INF("XML storage loaded");

	data::xmlCacheManager xmlCache;
	xmlCache.parse(conf::config::instance().cacheFile());
	utils::finally xmlCacheDump([&xmlCache]() { xmlCache.dump(conf::config::instance().cacheFile()); });
	LOG_INF("XML cache loaded");


	auto storage = xmlStorage.getStorage();
	auto cache = xmlCache.getCache();

	core::core::init();

	ui::uiImpl ui(cache, storage);
	LOG_INF("CLI created");
	ui.init();

	return ui.run();
}