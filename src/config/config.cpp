#include "config/config.h"

#include <memory>
#include <stdexcept>

#include "config/configData.h"

#include "utils/logger.h"

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
		LOG_INF("Setting custom config: " << *data);
	}
	else
	{
		LOG_INF("Setting default config: " << *data_);
	}
}

const std::string& config::executable() const
{
	return data_->executable;
}

const std::vector<std::string>& config::activated() const
{
	return data_->activated;
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
} // namespace tk