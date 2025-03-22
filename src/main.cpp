#include "cli/core/interface.h"
#include "cli/cli.h"

#include "clipboardController/interface.h"
#include "clipboardController/windowsClipboardController.h"

#include "config/config.h"

#include "storage/xmlStorageManager.h"
#include "storage/xmlCacheManager.h"

#include "utils/finally.h"

int main(int, char**)
{
	tk::config::instance().init();

	tk::clipboardController::shared_ptr_type clc = std::make_shared<tk::windowsClipboardController>();

	tk::xmlStorageManager xmlStorage;
	xmlStorage.parse(tk::config::instance().storageFile());
	tk::finally xmlStorageDump([&xmlStorage]() { xmlStorage.dump(tk::config::instance().storageFile()); });

	tk::xmlCacheManager xmlCache;
	xmlCache.parse(tk::config::instance().cacheFile());
	tk::finally xmlCacheDump([&xmlCache]() { xmlCache.dump(tk::config::instance().cacheFile()); });

	auto storage = xmlStorage.getStorage();
	auto cache = xmlCache.getCache();

	tk::cli::core::init();

	tk::cliImpl cli(clc, cache, storage);
	cli.init();

	return cli.run();
}