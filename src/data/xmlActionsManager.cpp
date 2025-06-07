#include "data/xmlActionsManager.h"
#include "data/actions.h"

#include <pugixml.hpp>

namespace data
{

std::shared_ptr<actions> ActionsXmlStorageManager::parse(const std::string& filename)
{
	auto actionsStorage = std::make_shared<data::actions>();

	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str()))
	{
		return nullptr;
	}

	auto rootNode = doc.child("actions");
	if (!rootNode)
	{
		return nullptr;
	}

	// Parse top-level actions
	for (auto actionNode : rootNode.children("action"))
	{
		parseAction(actionNode, actionsStorage->getRoot());
	}

	// Parse folders
	for (auto folderNode : rootNode.children("folder"))
	{
		parseFolder(folderNode, actionsStorage->getRoot());
	}

	return actionsStorage;
}

bool ActionsXmlStorageManager::dump(const std::string& filename, const std::shared_ptr<actions>& actionsData)
{
	if (!actionsData || !actionsData->getRoot())
	{
		return false;
	}

	pugi::xml_document doc;
	auto rootNode = doc.append_child("actions");

	// Dump top-level actions
	for (const auto& action : actionsData->getRoot()->getActions())
	{
		dumpAction(rootNode, action);
	}

	// Dump folders
	for (const auto& folder : actionsData->getRoot()->getChildren())
	{
		dumpFolder(rootNode, folder);
	}

	return doc.save_file(filename.c_str());
}

void ActionsXmlStorageManager::parseAction(const pugi::xml_node& actionNode, const std::shared_ptr<actionsFolder>& parentFolder)
{
	std::string uuid = actionNode.attribute("uuid").as_string();
	std::string alias = actionNode.child("alias").text().as_string();
	std::string content = actionNode.child("content").text().as_string();
	std::string description = actionNode.child("description").text().as_string();
	std::string typeStr = actionNode.child("type").text().as_string();

	action::type type = action::NONE;
	if (typeStr == "exec")
		type = action::EXECUTE_TERMINAL_COMMAND;
	else if (typeStr == "copy")
		type = action::COPY_TERMINAL_COMMAND;

	auto action = std::make_shared<data::action>(alias, content, description, type);
	action->setUuid(uuids::uuid::from_string(uuid).value());
	parentFolder->addAction(action);
}

void ActionsXmlStorageManager::parseFolder(const pugi::xml_node& folderNode, const std::shared_ptr<actionsFolder>& parentFolder)
{
	std::string uuid = folderNode.attribute("uuid").as_string();
	std::string name = folderNode.child("name").text().as_string();
	std::string description = ""; // Assuming description is optional

	auto folder = std::make_shared<actionsFolder>(name, description);
	folder->setUuid(uuids::uuid::from_string(uuid).value());
	parentFolder->addFolder(folder);

	// Parse actions in folder
	auto actionsNode = folderNode.child("actions");
	if (actionsNode)
	{
		for (auto actionNode : actionsNode.children("action"))
		{
			parseAction(actionNode, folder);
		}
	}

	// Parse subfolders
	for (auto subFolderNode : folderNode.children("folder"))
	{
		parseFolder(subFolderNode, folder);
	}
}

void ActionsXmlStorageManager::dumpAction(pugi::xml_node& parentNode, const std::shared_ptr<action>& action)
{
	auto actionNode = parentNode.append_child("action");
	actionNode.append_attribute("uuid").set_value(uuids::to_string(action->uuid()).c_str());

	actionNode.append_child("alias").text().set(action->getAlias().c_str());
	actionNode.append_child("content").text().set(action->getContent().c_str());
	actionNode.append_child("description").text().set(action->getDescription().c_str());

	std::string typeStr;
	switch (action->getType())
	{
		case action::EXECUTE_TERMINAL_COMMAND: typeStr = "exec"; break;
		case action::COPY_TERMINAL_COMMAND: typeStr = "copy"; break;
		default: typeStr = "nonde";
	}
	actionNode.append_child("type").text().set(typeStr.c_str());
}

void ActionsXmlStorageManager::dumpFolder(pugi::xml_node& parentNode, const std::shared_ptr<actionsFolder>& folder)
{
	auto folderNode = parentNode.append_child("folder");
	folderNode.append_attribute("uuid").set_value(uuids::to_string(folder->uuid()).c_str());

	folderNode.append_child("name").text().set(folder->getName().c_str());
	folderNode.append_child("description").text().set(folder->getDescription().c_str());

	// Dump actions in folder
	if (!folder->getActions().empty())
	{
		auto actionsNode = folderNode.append_child("actions");
		for (const auto& action : folder->getActions())
		{
			dumpAction(actionsNode, action);
		}
	}

	// Dump subfolders
	for (const auto& subFolder : folder->getChildren())
	{
		dumpFolder(folderNode, subFolder);
	}
}

} // namespace data