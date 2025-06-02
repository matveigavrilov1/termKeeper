#pragma once

#include <atomic>
#include <functional>

#include "core/event.h"
#include "utils/pcQueue.h"

namespace core
{

class eventm
{
public:
	int run();
	void stop();
	void loop();

	using event_handler_t = std::function<void(event::shared_ptr_t)>;
	using event_queue_t = utils::pcQueue<std::pair<event::shared_ptr_t, event_handler_t>>;

	void pushEvent(event::shared_ptr_t event, event_handler_t handler);

private:
	std::atomic<bool> running_ { false };

	event_queue_t eventQueue_;
};
} // namespace core