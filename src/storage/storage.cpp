#include "storage/storage.h"

namespace tk
{

storage::storage()
{
	root_ = std::make_shared<folder>("/");
	currentFolder_ = root_;
}

std::shared_ptr<storage::folder> storage::root() const
{
	return root_;
}

std::shared_ptr<storage::folder> storage::currentFolder() const
{
	return currentFolder_;
}

bool storage::curIsRoot() const
{
	return root_.get() == currentFolder_.get();
}

void storage::setRoot()
{
	currentFolder_ = root_;
}

void storage::folderUp()
{
	if (currentFolder_ != root_)
	{
		currentFolder_ = currentFolder_->parent_.lock();
	}
}

void storage::folderDown(const std::string& name)
{
	auto it = currentFolder_->subFolders_.find(name);
	if (it != currentFolder_->subFolders_.end())
	{
		currentFolder_ = it->second;
	}
}

void storage::addFolder(const std::string& name)
{
	auto newFolder = std::make_shared<folder>(name);
	newFolder->parent_ = currentFolder_;
	currentFolder_->subFolders_[name] = newFolder;
}

void storage::addCommand(const std::string& command)
{
	currentFolder_->commands_.push_back(command);
}


} // namespace tk