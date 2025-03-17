#pragma once

#include <atomic>
#include <functional>

#include "cli/core/event.h"
#include "utils/pcQueue.h"

namespace tk
{

class eventm
{
public:
	void run();
	void stop();
	void loop();

	using event_handler_type = std::function<void(event::shared_ptr_type)>;
	using event_queue_type = pcQueue<std::pair<event::shared_ptr_type, event_handler_type>>;

	void pushEvent(event::shared_ptr_type event, event_handler_type handler);

private:
	std::atomic<bool> running_ { false };

	event_queue_type eventQueue_;
};
} // namespace tk