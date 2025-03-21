#include "storage/xmlStorageManager.h"

namespace tk
{
xmlStorageManager::xmlStorageManager()
: storage_(std::make_shared<storage>())
{ }

std::shared_ptr<storage> xmlStorageManager::getStorage() const
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
		storage_->root()->commands_.push_back(commandNode.text().as_string());
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
		commandNode.text().set(command.c_str());
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
		folder->subFolders_[folderName] = newFolder;

		for (auto commandNode : subFolderNode.children("command"))
		{
			newFolder->commands_.push_back(commandNode.text().as_string());
		}

		parseFolder(subFolderNode, newFolder);
	}
}

void xmlStorageManager::dumpFolder(pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder)
{
	for (const auto& [name, subFolder] : folder->subFolders_)
	{
		auto subFolderNode = xmlNode.append_child("folder");
		subFolderNode.append_attribute("name").set_value(name.c_str());

		for (const auto& command : subFolder->commands_)
		{
			auto commandNode = subFolderNode.append_child("command");
			commandNode.text().set(command.c_str());
		}

		dumpFolder(subFolderNode, subFolder);
	}
}
} // namespace tk