#pragma once

#include <memory>
#include <string>

namespace tk
{
class clipboardController
{
public:
	using s_ptr_t = std::shared_ptr<clipboardController>;
	virtual ~clipboardController() = default;
	virtual void write(const std::string& content) = 0;
};
} // namespace tk