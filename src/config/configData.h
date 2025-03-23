#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace tk
{
class config;

class configData
{
public:
	using unique_ptr_type = std::unique_ptr<configData>;
	bool load(const std::string& path);

	friend class config;

	using hints_type = std::vector<std::pair<std::string, std::string>>;

private:
	// tkHotkeyBinder
	std::string executable { "termKeeper.exe" };
	std::string hotKey { "Ctrl+Alt+R" };

	// tkExecutable
	bool closeOnChoice { true };
	std::vector<std::string> registrated { "Menu", "Storage", "Cache" };
	std::vector<std::string> activated { "Menu", "Storage" };
	std::vector<std::string> menu { "Storage", "Cache" };
	std::string initialController { "Menu" };
	std::string storageFile { "storage.xml" };
	std::string cacheFile { "cache.xml" };
	size_t cacheMaxSize { 10 };

	size_t screenWidth { 0 };
	size_t screenHeight { 0 };

	std::map<std::string, hints_type> hintsPresets;
};
} // namespace tk