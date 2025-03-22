#pragma once

#include <string>

#include "config/configData.h"

#include "utils/singleton.h"

namespace tk
{
class config : public singleton<config>
{
public:
	void init();

	const std::string& executable() const;
    const std::vector<std::string>& activated() const;
	const std::string& initialController() const;
	const std::string& storageFile() const;
	const std::string& cacheFile() const;
	size_t cacheMaxSize() const;

private:
	configData::unique_ptr_type data_ { std::make_unique<configData>() };
	std::string configFile { "settings.yaml" };
};
} // namespace tk