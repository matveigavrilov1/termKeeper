#include "cli/core/interface.h"
#include "cli/cli.h"

#include "clipboardController/interface.h"
#include "clipboardController/windowsClipboardController.h"

#include "config/config.h"

#include "storage/xmlStorageManager.h"
#include "storage/xmlCacheManager.h"

#include "utils/finally.h"
#include "utils/logger.h"

#include <cstdlib>

bool setConsoleWindowSize(int width, int height)
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Cannot get console window", "Error", MB_ICONERROR);
		return false;
	}

	RECT rect;
	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, rect.left, rect.top, width, height, TRUE);
	return true;
}

int main(int, char**)
{
	tk::config::instance().init();

	auto screenWidth = tk::config::instance().screenWidth();
	auto screenHeight = tk::config::instance().screenHeight();
	if (screenWidth && screenHeight && !setConsoleWindowSize(screenWidth, screenHeight))
	{
		LOG_ERR("Failed to set screen size");
		return EXIT_FAILURE;
	}

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