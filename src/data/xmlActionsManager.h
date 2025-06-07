#pragma once

#include <memory>
#include <string>

#include <pugixml.hpp>

#include "data/actions.h"

namespace data
{

class ActionsXmlStorageManager
{
public:
	static std::shared_ptr<actions> parse(const std::string& filename);
	static bool dump(const std::string& filename, const std::shared_ptr<actions>& actionsData);

private:
	static void parseAction(const pugi::xml_node& actionNode, const std::shared_ptr<actionsFolder>& parentFolder);
	static void parseFolder(const pugi::xml_node& folderNode, const std::shared_ptr<actionsFolder>& parentFolder);
	static void dumpAction(pugi::xml_node& parentNode, const std::shared_ptr<action>& action);
	static void dumpFolder(pugi::xml_node& parentNode, const std::shared_ptr<actionsFolder>& folder);
};

} // namespace data
