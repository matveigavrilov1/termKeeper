#pragma once

#include <memory>
#include <string>

#include <pugixml.hpp>

#include "storage/storage.h"

namespace tk
{
class xmlStorageManager
{
public:
	xmlStorageManager();
	std::shared_ptr<storage> getStorage() const;

	bool parse(const std::string& filename);
	bool dump(const std::string& filename);

private:
	void parseFolder(const pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder);
	void dumpFolder(pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder);

	std::shared_ptr<storage> storage_;
};
} // namespace tk