#include "core/interface.h"
#include "ui/ui.h"

#include "config/config.h"

#include "os/console.h"

#include "os/interface.h"
#include "storage/xmlStorageManager.h"
#include "storage/xmlCacheManager.h"

#include "utils/finally.h"
#include "utils/logger.h"

#include <cstdlib>

std::atomic<bool> running = true;

int main(int, char**)
{
	LOG_INF("Starting project controller");
	std::thread([&]() { os::runApp("project-controller", running); }).detach();
	LOG_INF("Event loop started");

	tk::config::instance().init();
	LOG_INF("Config initialized");

	auto screenWidth = tk::config::instance().screenWidth();
	auto screenHeight = tk::config::instance().screenHeight();
	if (screenWidth && screenHeight && !os::console::get()->setConsoleSize({ screenWidth, screenHeight }))
	{
		LOG_ERR("Failed to set screen size");
		return EXIT_FAILURE;
	}
	LOG_INF("Window size set");

	tk::xmlStorageManager xmlStorage;
	xmlStorage.parse(tk::config::instance().storageFile());
	tk::finally xmlStorageDump([&xmlStorage]() { xmlStorage.dump(tk::config::instance().storageFile()); });
	LOG_INF("XML storage loaded");

	tk::xmlCacheManager xmlCache;
	xmlCache.parse(tk::config::instance().cacheFile());
	tk::finally xmlCacheDump([&xmlCache]() { xmlCache.dump(tk::config::instance().cacheFile()); });
	LOG_INF("XML cache loaded");


	auto storage = xmlStorage.getStorage();
	auto cache = xmlCache.getCache();

	tk::ui::core::init();

	tk::uiImpl ui(cache, storage);
	LOG_INF("CLI created");
	ui.init();

	return ui.run();
}