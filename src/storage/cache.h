#pragma once

#include <memory>
#include <string>
#include <vector>
#include <uuid.h>

#include "utils/subject.h"
#include "utils/generate_uuid.h"

namespace tk
{
class cache : public subject
{
public:
	using shared_ptr_t = std::shared_ptr<cache>;

	struct cache_item
	{
		std::string content;
		uuids::uuid uuid;

		using ptr_t = std::shared_ptr<cache_item>;

		static ptr_t create(const std::string& content) { return std::make_shared<cache_item>(cache_item { content, utils::generate_uuid() }); }

		bool operator== (const uuids::uuid& other) const { return uuid == other; }
	};

	using item_ptr_t = cache_item::ptr_t;
	using vector_t = std::vector<item_ptr_t>;

	explicit cache(size_t maxSize);

	const vector_t& getCache() const;
	item_ptr_t findItem(const uuids::uuid& uuid) const;

	item_ptr_t pushFront(const std::string& content);
	item_ptr_t pushBack(const std::string& content);

	bool updateItem(const uuids::uuid& uuid, const std::string& new_content);
	bool removeItem(const uuids::uuid& uuid);

	void setMaxSize(size_t maxSize);

private:
	vector_t items_;
	size_t maxSize_;
};
} // namespace tk
