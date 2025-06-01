#include "cache.h"
#include "utils/subject.h"

#include <algorithm>

namespace tk
{

cache::cache(size_t maxSize)
: maxSize_(maxSize)
{ }

const cache::vector_t& cache::getCache() const
{
	return items_;
}

cache::item_ptr_t cache::findItem(const uuids::uuid& uuid) const
{
	auto it = std::find_if(items_.begin(), items_.end(), [&uuid](const item_ptr_t& item) { return item->uuid == uuid; });

	return it != items_.end() ? *it : nullptr;
}

cache::item_ptr_t cache::pushFront(const std::string& content)
{
	auto it = std::find_if(items_.begin(), items_.end(), [&content](const item_ptr_t& item) { return item->content == content; });

	item_ptr_t item;

	if (it != items_.end())
	{
		item = *it;
		items_.erase(it);
	}
	else
	{

		item = cache_item::create(content);
	}

	items_.insert(items_.begin(), item);

	if (items_.size() > maxSize_)
	{
		items_.resize(maxSize_);
	}

	subject::notify("Cache content update");
	return item;
}

cache::item_ptr_t cache::pushBack(const std::string& content)
{
	// Check if item with same content already exists
	auto it = std::find_if(items_.begin(), items_.end(), [&content](const item_ptr_t& item) { return item->content == content; });

	item_ptr_t item;

	if (it != items_.end())
	{
		// Move existing item to back
		item = *it;
		items_.erase(it);
	}
	else
	{
		// Create new item
		item = cache_item::create(content);
	}

	items_.push_back(item);

	// Trim if exceeds max size
	if (items_.size() > maxSize_)
	{
		items_.resize(maxSize_);
	}

	subject::notify("Cache content update");
	return item;
}

bool cache::updateItem(const uuids::uuid& uuid, const std::string& new_content)
{
	auto item = findItem(uuid);
	if (item)
	{
		item->content = new_content;
		subject::notify("Cache item updated");
		return true;
	}
	return false;
}

bool cache::removeItem(const uuids::uuid& uuid)
{
	auto it = std::find_if(items_.begin(), items_.end(), [&uuid](const item_ptr_t& item) { return item->uuid == uuid; });

	if (it != items_.end())
	{
		items_.erase(it);
		subject::notify("Cache item removed");
		return true;
	}
	return false;
}

void cache::setMaxSize(size_t maxSize)
{
	maxSize_ = maxSize;

	if (items_.size() > maxSize_)
	{
		items_.resize(maxSize_);
	}

	subject::notify("Cache max size update");
}

} // namespace tk