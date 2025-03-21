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
	storage::shared_ptr_type getStorage() const;

	bool parse(const std::string& filename);
	bool dump(const std::string& filename);

private:
	void parseFolder(const pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder);
	void dumpFolder(pugi::xml_node& xmlNode, std::shared_ptr<storage::folder> folder);

	storage::shared_ptr_type storage_;
};
} // namespace tk