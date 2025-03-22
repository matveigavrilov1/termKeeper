#pragma once

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
	friend std::ostream& operator<< (std::ostream& os, configData& data);

private:
	// tkHotkeyBinder
	std::string executable { "termKeeper.exe" };

	// tkExecutable
	std::vector<std::string> activated { "Menu", "Storage", "Cache" };
	std::string initialController { "Menu" };
	std::string storageFile { "storage.xml" };
	std::string cacheFile { "cache.xml" };
	size_t cacheMaxSize { 10 };
};

std::ostream& operator<< (std::ostream& os, configData& data);
} // namespace tk