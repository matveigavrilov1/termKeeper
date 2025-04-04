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
	const std::string& hotKey() const;

	bool closeOnChoice() const;
	const std::vector<std::string>& registrated() const;
	const std::vector<std::string>& activated() const;
	const std::vector<std::string>& menu() const;
	const std::string& initialController() const;
	const std::string& storageFile() const;
	const std::string& cacheFile() const;
	size_t cacheMaxSize() const;
	size_t screenWidth() const;
	size_t screenHeight() const;

	configData::hints_type hintsPreset(const std::string& name) const;

private:
	configData::unique_ptr_type data_ { std::make_unique<configData>() };
	std::string configFile { "settings.yaml" };
};
} // namespace tk