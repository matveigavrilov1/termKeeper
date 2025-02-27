#include "model.h"
#include "utils/logger.h"
#include <memory>
#include <stdexcept>

using namespace tk;

std::string commandImpl::getContent() const
{
	return content_;
}

const command::s_ptr_vec_t& commandFolderImpl::getCommands() const
{
	return commands_;
}

const commandFolder::folders_map_t& commandFolderImpl::getFolders() const
{
	return folders_;
}

std::string commandFolderImpl::getName() const
{
	return name_;
}

void dataModelImpl::changeFolderUp()
{
	if (currentFolder_ == nullptr)
	{
		LOG_ERR("Current folder has no value");
		return;
	}
	if (currentFolder_->parent_ == nullptr)
	{
		LOG_WRN("Trying to go up from folder with no parent");
		return;
	}
	currentFolder_ = std::dynamic_pointer_cast<commandFolderImpl>(currentFolder_->parent_);
}

void dataModelImpl::changeFolderDown(const std::string& folderName)
{
	if (currentFolder_ == nullptr)
	{
		LOG_ERR("Current folder has no value");
		return;
	}
	auto it = currentFolder_->folders_.find(folderName);
	if (it == currentFolder_->folders_.end())
	{
		LOG_WRN("Folder with name " << folderName << " not found");
		return;
	}
	currentFolder_ = std::dynamic_pointer_cast<commandFolderImpl>(it->second);
}

const commandFolder& dataModelImpl::getCurrentFolder() const
{
	if (currentFolder_ == nullptr)
	{
		LOG_ERR("Current folder has no value");
		throw std::runtime_error("Current folder has no value");
	}
	return *currentFolder_;
}

void dataModelImpl::addToCache(command::s_ptr_t command)
{
	cache_.push_back(command);
	if (cache_.size() > MAX_CACHE_SIZE)
	{
		cache_.pop_front();
	}
}

const dataModel::cache_t& dataModelImpl::getCache() const
{
	return cache_;
}

void dataModelImpl::clearCache()
{
	cache_.clear();
}
