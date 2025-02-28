#pragma once

#include <memory>
#include <windows.h>

namespace tk
{
enum eventType
{
	NONE_EVENT,
	EXIT_EVENT,
	INPUT_EVENT,
	WINDOW_ACTIVATE_EVENT,
	CHANGE_ACTIVE_WINDOW_EVENT,
	SHOW_WINDOW_EVENT,
	SHOW_SCREEN_EVENT
};

class eventData
{
public:
	virtual ~eventData() = default;
	using unique_ptr_type = std::unique_ptr<eventData>;
	using shared_ptr_type = std::shared_ptr<eventData>;
};

class event
{
public:
	event(eventType type);
	event(eventType type, eventData::shared_ptr_type data);
	virtual ~event() = default;

	
	using unique_ptr_type = std::unique_ptr<event>;
	using shared_ptr_type = std::shared_ptr<event>;
	
	virtual void defaultHandler(shared_ptr_type event) = 0;

	eventType type();
	eventData::shared_ptr_type data();

private:
	eventType type_ { NONE_EVENT };
	eventData::shared_ptr_type data_ { nullptr };
};
} // namespace tk