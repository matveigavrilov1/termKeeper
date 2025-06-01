#include "storage/storage.h"
#include "utils/generate_uuid.h"

#include <algorithm>
#include <memory>

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
	if (currentFolder_ != root_ && !currentFolder_->parent_.expired())
	{
		currentFolder_ = currentFolder_->parent_.lock();
	}
}

void storage::folderDown(const uuids::uuid& folder_uuid)
{
	auto it = std::find_if(
		currentFolder_->subFolders_.begin(), currentFolder_->subFolders_.end(), [&folder_uuid](const auto& pair) { return pair.second->uuid_ == folder_uuid; });

	if (it != currentFolder_->subFolders_.end())
	{
		currentFolder_ = it->second;
	}
}

uuids::uuid storage::addFolder(const std::string& name)
{
	auto newFolder = std::make_shared<folder>(name);
	newFolder->parent_ = currentFolder_;
	currentFolder_->subFolders_[newFolder->uuid_] = newFolder;
	return newFolder->uuid_;
}

uuids::uuid storage::addCommand(const std::string& command)
{
	auto cmd = std::make_shared<command_t>(command, utils::generate_uuid());
	currentFolder_->commands_.push_back(cmd);
	return cmd->uuid;
}

void storage::deleteFolder(const uuids::uuid& folder_uuid)
{
	auto it = currentFolder_->subFolders_.find(folder_uuid);
	if (it != currentFolder_->subFolders_.end())
	{
		currentFolder_->subFolders_.erase(it);
	}
}

void storage::deleteCommand(const uuids::uuid& command_uuid)
{
	auto& commands = currentFolder_->commands_;
	commands.erase(std::remove_if(commands.begin(), commands.end(), [&command_uuid](const auto& cmd) { return cmd->uuid == command_uuid; }), commands.end());
}

void storage::renameFolder(const uuids::uuid& folder_uuid, const std::string& newName)
{
	auto it = currentFolder_->subFolders_.find(folder_uuid);
	if (it != currentFolder_->subFolders_.end())
	{
		it->second->name_ = newName;
	}
}

void storage::editCommand(const uuids::uuid& command_uuid, const std::string& newCommand)
{
	auto& commands = currentFolder_->commands_;
	auto it = std::find_if(commands.begin(), commands.end(), [&command_uuid](const auto& cmd) { return cmd->uuid == command_uuid; });

	if (it != commands.end())
	{
		(*it)->content = newCommand;
	}
}

const storage::folder_shared_ptr_t storage::findFolder(const uuids::uuid& uuid) const
{
	return findFolderImpl(root_, uuid);
}

const storage::command_shared_ptr_t storage::findCommand(const uuids::uuid& uuid) const
{
	// Search in current folder first
	auto it = std::find_if(currentFolder_->commands_.begin(), currentFolder_->commands_.end(), [&uuid](const auto& cmd) { return cmd->uuid == uuid; });

	if (it != currentFolder_->commands_.end())
	{
		return (*it);
	}

	// If not found, search recursively in all folders
	folder_shared_ptr_t folder_with_command;
	std::function<bool(const folder_shared_ptr_t&)> search = [&](const folder_shared_ptr_t& f)
	{
		auto cmd_it = std::find_if(f->commands_.begin(), f->commands_.end(), [&uuid](const auto& cmd) { return cmd->uuid == uuid; });

		if (cmd_it != f->commands_.end())
		{
			folder_with_command = f;
			return true;
		}

		for (const auto& [_, subfolder] : f->subFolders_)
		{
			if (search(subfolder))
				return true;
		}

		return false;
	};

	if (search(root_))
	{
		auto cmd_it =
			std::find_if(folder_with_command->commands_.begin(), folder_with_command->commands_.end(), [&uuid](const auto& cmd) { return cmd->uuid == uuid; });
		if (cmd_it != folder_with_command->commands_.end())
		{
			return (*cmd_it);
		}
	}

	return nullptr;
}

storage::folder_shared_ptr_t storage::findFolderImpl(const folder_shared_ptr_t& current, const uuids::uuid& uuid) const
{
	if (current->uuid_ == uuid)
	{
		return current;
	}

	for (const auto& [_, subfolder] : current->subFolders_)
	{
		auto found = findFolderImpl(subfolder, uuid);
		if (found)
		{
			return found;
		}
	}

	return nullptr;
}

} // namespace tk