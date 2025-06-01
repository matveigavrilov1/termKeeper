#include "storage/xmlStorageManager.h"
#include "utils/generate_uuid.h"

namespace tk
{
xmlStorageManager::xmlStorageManager()
: storage_(std::make_shared<storage>())
{ }

storage::shared_ptr_type xmlStorageManager::getStorage() const
{
	return storage_;
}

bool xmlStorageManager::parse(const std::string& filename)
{
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str()))
	{
		return false;
	}

	auto rootNode = doc.child("storage");
	if (!rootNode)
	{
		return false;
	}

	for (auto commandNode : rootNode.children("command"))
	{
		auto command = std::make_shared<storage::command_t>(commandNode.text().as_string(), utils::generate_uuid());
		storage_->root()->commands_.push_back(command);
	}

	parseFolder(rootNode, storage_->root());
	return true;
}

bool xmlStorageManager::dump(const std::string& filename)
{
	pugi::xml_document doc;
	auto storageNode = doc.append_child("storage");

	for (const auto& command : storage_->root()->commands_)
	{
		auto commandNode = storageNode.append_child("command");
		commandNode.text().set(command->content.c_str());
	}

	dumpFolder(storageNode, storage_->root());

	if (!doc.save_file(filename.c_str()))
	{
		return false;
	}

	return true;
}

void xmlStorageManager::parseFolder(const pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder)
{
	for (auto subFolderNode : xmlNode.children("folder"))
	{
		std::string folderName = subFolderNode.attribute("name").as_string();
		auto newFolder = std::make_shared<storage::folder>(folderName);
		newFolder->parent_ = folder;
		folder->subFolders_[utils::generate_uuid()] = newFolder;

		for (auto commandNode : subFolderNode.children("command"))
		{
			auto command = std::make_shared<storage::command_t>(commandNode.text().as_string(), utils::generate_uuid());
			newFolder->commands_.push_back(command);
		}

		parseFolder(subFolderNode, newFolder);
	}
}

void xmlStorageManager::dumpFolder(pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder)
{
	for (const auto& [name, subFolder] : folder->subFolders_)
	{
		auto subFolderNode = xmlNode.append_child("folder");
		subFolderNode.append_attribute("name").set_value(subFolder->name_.c_str());

		for (const auto& command : subFolder->commands_)
		{
			auto commandNode = subFolderNode.append_child("command");
			commandNode.text().set(command->content.c_str());
		}

		dumpFolder(subFolderNode, subFolder);
	}
}
} // namespace tk