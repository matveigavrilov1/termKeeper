#pragma once

#include "cli/core/window.h"

namespace tk
{
class form
{
public:
	form(size_t x, size_t y, size_t width, size_t height);
	virtual ~form() = default;

	virtual void show(window& wnd) = 0;

protected:
	size_t x_ { 0 }, y_ { 0 }, width_ { 0 }, height_ { 0 };
};
} // namespace tk