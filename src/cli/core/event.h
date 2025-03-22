#pragma once

#include <memory>
#include <windows.h>

namespace tk
{
enum eventType
{
	NONE_EVENT = 0,
	EXIT_EVENT = 1,
	INPUT_EVENT = 2,
	WINDOW_EVENT = 3,
	SCREEN_EVENT = 4
};

class event
{
public:
	event(unsigned type);

	virtual ~event() = default;

	using unique_ptr_type = std::unique_ptr<event>;
	using shared_ptr_type = std::shared_ptr<event>;

	unsigned type();

private:
	unsigned type_ { NONE_EVENT };
};
} // namespace tk