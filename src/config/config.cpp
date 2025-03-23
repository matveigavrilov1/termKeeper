#include "config/config.h"

#include <memory>
#include <stdexcept>

#include "config/configData.h"

namespace tk
{
void config::init()
{
	auto data = std::make_unique<configData>();
	if (!data)
	{
		throw std::runtime_error("Failed to init config");
	}

	if (data->load(configFile))
	{
		data_ = std::move(data);
	}
}

const std::string& config::executable() const
{
	return data_->executable;
}

const std::string& config::hotKey() const
{
	return data_->hotKey;
}

const std::vector<std::string>& config::registrated() const
{
	return data_->registrated;
}

const std::vector<std::string>& config::activated() const
{
	return data_->activated;
};

const std::vector<std::string>& config::menu() const
{
	return data_->menu;
};

const std::string& config::initialController() const
{
	return data_->initialController;
}

const std::string& config::storageFile() const
{
	return data_->storageFile;
}

const std::string& config::cacheFile() const
{
	return data_->cacheFile;
}

size_t config::cacheMaxSize() const
{
	return data_->cacheMaxSize;
}

size_t config::screenWidth() const
{
	return data_->screenWidth;
}

size_t config::screenHeight() const
{
	return data_->screenWidth;
}
} // namespace tk