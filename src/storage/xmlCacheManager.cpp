#include "storage/xmlCacheManager.h"

#include <memory>

#include <pugixml.hpp>

namespace tk
{
xmlCacheManager::xmlCacheManager()
: cache_(std::make_shared<cache>(10))
{ }

cache::shared_ptr_type xmlCacheManager::getCache() const
{
	return cache_;
};

bool xmlCacheManager::parse(const std::string& filename)
{
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str()))
	{
		return false;
	}

	auto cacheNode = doc.child("cache");
	if (!cacheNode)
	{
		return false;
	}

	for (auto commandNode : cacheNode.children("command"))
	{
		cache_->pushBack(commandNode.text().as_string());
	}

	return true;
}

bool xmlCacheManager::dump(const std::string& filename)
{
	pugi::xml_document doc;
	auto cacheNode = doc.append_child("cache");

	for (const auto& command : cache_->getCache())
	{
		auto commandNode = cacheNode.append_child("command");
		commandNode.text().set(command.c_str());
	}

	if (!doc.save_file(filename.c_str()))
	{
		return false;
	}

	return true;
}
} // namespace tk