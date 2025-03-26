#pragma once

#include <memory>
#include <string>
#include <vector>

#include "utils/subject.h"

namespace tk
{
class cache : public subject
{
public:
	using shared_ptr_type = std::shared_ptr<cache>;
	using item_type = std::string;
	using vector_type = std::vector<std::string>;

	explicit cache(size_t maxSize);

	vector_type& getCache();

	void pushFront(const item_type& item);
	void pushBack(const item_type& item);

	void setMaxSize(size_t maxSize);

private:
	vector_type items_;
	size_t maxSize_;
};
} // namespace tk