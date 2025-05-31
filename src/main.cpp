#include "cli/core/interface.h"
#include "cli/cli.h"

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
	tk::config::instance().init();

	auto screenWidth = tk::config::instance().screenWidth();
	auto screenHeight = tk::config::instance().screenHeight();
	if (screenWidth && screenHeight && !os::console::get()->setConsoleSize({screenWidth, screenHeight}))
	{
		LOG_ERR("Failed to set screen size");
		return EXIT_FAILURE;
	}

	tk::xmlStorageManager xmlStorage;
	xmlStorage.parse(tk::config::instance().storageFile());
	tk::finally xmlStorageDump([&xmlStorage]() { xmlStorage.dump(tk::config::instance().storageFile()); });

	tk::xmlCacheManager xmlCache;
	xmlCache.parse(tk::config::instance().cacheFile());
	tk::finally xmlCacheDump([&xmlCache]() { xmlCache.dump(tk::config::instance().cacheFile()); });

	auto storage = xmlStorage.getStorage();
	auto cache = xmlCache.getCache();

	tk::cli::core::init();

	tk::cliImpl cli(cache, storage);
	cli.init();
	cli.run();

	return os::runApp("project-controller", running);
}