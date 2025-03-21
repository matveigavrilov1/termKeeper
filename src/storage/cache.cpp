#include "cache.h"
#include "utils/subject.h"

#include <algorithm>

namespace tk
{

cache::cache(size_t maxSize)
: maxSize_(maxSize)
{ }

cache::vector_type& cache::getCache()
{
	return items_;
}

void cache::addItem(const item_type& item)
{
	auto it = std::find(items_.begin(), items_.end(), item);

	if (it != items_.end())
	{
		items_.erase(it);
	}

	items_.insert(items_.begin(), item);

	if (items_.size() > maxSize_)
	{
		items_.resize(maxSize_);
	}

	subject::notify("Cache content update");
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