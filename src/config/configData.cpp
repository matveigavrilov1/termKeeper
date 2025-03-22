#include "config/configData.h"

#include <cstddef>
#include <ostream>

#include <yaml-cpp/yaml.h>

#include "utils/logger.h"

namespace tk
{

bool configData::load(const std::string& path)
{
	try
	{
		YAML::Node config = YAML::LoadFile(path);

		// tkHotKeyBinder
		auto tkHotKeyBinder = config["tkHotKeyBinder"];
		if (tkHotKeyBinder)
		{
			executable = tkHotKeyBinder["executable"].as<std::string>();
		}

		// tkExecutable
		auto tkExecutable = config["tkExecutable"];
		if (!tkExecutable)
		{
			return true;
		}

		if (tkExecutable["windows"])
		{
			auto windowsNode = tkExecutable["windows"];

			if (windowsNode["activated"])
			{
				for (const auto& item : windowsNode["activated"])
				{
					activated.push_back(item.as<std::string>());
				}
			}

			if (windowsNode["initialController"])
			{
				initialController = windowsNode["initialController"].as<std::string>();
			}
		}

		if (tkExecutable["storage"])
		{
			storageFile = tkExecutable["storage"]["storageFile"].as<std::string>();
		}

		if (tkExecutable["cache"])
		{
			cacheFile = config["tkExecutable"]["cache"]["cacheFile"].as<std::string>();
			cacheMaxSize = config["tkExecutable"]["cache"]["cacheMaxSize"].as<size_t>();
		}
	}

	catch (const YAML::Exception& e)
	{
		LOG_WRN("Failed to load config: " << path << ". Error: " << e.what());
		return false;
	}

	return true;
}

std::ostream& operator<< (std::ostream& os, configData& data)
{
	os << "configData:" << std::endl;
	os << "tkHotKeyBinder:" << std::endl;
	os << "  executable:" << data.executable;
	return os;
}
} // namespace tk