#pragma once

#include <memory>
#include <string>

namespace tk
{
class clipboardController
{
public:
	using shared_ptr_type = std::shared_ptr<clipboardController>;
	virtual ~clipboardController() = default;
	virtual void write(const std::string& content) = 0;
};
} // namespace tk