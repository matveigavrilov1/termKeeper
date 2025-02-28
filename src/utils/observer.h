#pragma once

#include <memory>
#include <string>

namespace tk
{
class observer
{
public:
	virtual ~observer() = default;
	virtual void update(const std::string& message) = 0;

	using unique_ptr_type = std::unique_ptr<observer>;
	using shared_ptr_type = std::shared_ptr<observer>;
};
} // namespace tk