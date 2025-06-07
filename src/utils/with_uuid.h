#pragma once

#include <uuid.h>
#include <utils/generate_uuid.h>

namespace utils
{
class with_uuid
{
public:
	with_uuid(uuids::uuid uuid = utils::generate_uuid())
	: uuid_ { uuid }
	{ }

	with_uuid(const with_uuid& other) { uuid_ = utils::generate_uuid(); }

	with_uuid(with_uuid&& other) noexcept { uuid_ = other.uuid_; }

	with_uuid& operator= (const with_uuid& other)
	{
		uuid_ = utils::generate_uuid();
		return *this;
	}

	with_uuid& operator= (with_uuid&& other) noexcept
	{
		uuid_ = other.uuid_;
		return *this;
	}

	const uuids::uuid& uuid() const { return uuid_; }

	void setUuid(uuids::uuid uuid = utils::generate_uuid()) { uuid_ = uuid; }

private:
	uuids::uuid uuid_;
};
} // namespace utils