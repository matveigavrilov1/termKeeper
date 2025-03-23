#include "config/configData.h"

#include <cstddef>

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
		if (!tkHotKeyBinder)
		{
			return true;
		}
		if (tkHotKeyBinder["executable"])
		{
			executable = tkHotKeyBinder["executable"].as<std::string>();
		}
		if (tkHotKeyBinder["hotKey"])
		{
			hotKey = tkHotKeyBinder["hotKey"].as<std::string>();
		}

		// tkExecutable
		auto tkExecutable = config["tkExecutable"];
		if (!tkExecutable)
		{
			return true;
		}

		if (tkExecutable["closeOnChoice"])
		{
			closeOnChoice = tkExecutable["closeOnChoice"].as<bool>();
		}

		if (tkExecutable["windows"])
		{
			auto windowsNode = tkExecutable["windows"];

			if (windowsNode["registrated"])
			{
				registrated.clear();
				for (const auto& item : windowsNode["registrated"])
				{
					registrated.push_back(item.as<std::string>());
				}
			}

			if (windowsNode["activated"])
			{
				activated.clear();
				for (const auto& item : windowsNode["activated"])
				{
					activated.push_back(item.as<std::string>());
				}
			}

			if (windowsNode["menu"])
			{
				menu.clear();
				for (const auto& item : windowsNode["menu"])
				{
					menu.push_back(item.as<std::string>());
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

		if (tkExecutable["screenSize"])
		{
			if (!tkExecutable["screenSize"]["height"] || !tkExecutable["screenSize"]["width"])
			{
				LOG_ERR("Screen size is incorrect");
				return false;
			}

			screenWidth = tkExecutable["screenSize"]["width"].as<size_t>();
			screenHeight = tkExecutable["screenSize"]["height"].as<size_t>();
		}

		if (tkExecutable["hintsPresets"])
		{
			auto hintsPresetsNode = tkExecutable["hintsPresets"];
			for (const auto& preset : hintsPresetsNode)
			{
				const std::string& presetName = preset.first.as<std::string>();
				hints_type hints;

				for (const auto& hint : preset.second)
				{
					hints.push_back({ hint.first.as<std::string>(), hint.second.as<std::string>() });
				}

				hintsPresets[presetName] = hints;
			}
		}
	}

	catch (const YAML::Exception& e)
	{
		LOG_WRN("Failed to load config: " << path << ". Error: " << e.what());
		return false;
	}

	return true;
}

} // namespace tk