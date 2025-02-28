#include "cli/core/eventm.h"

namespace tk
{

void eventm::run()
{
	if (running_)
	{
		return;
	}
	running_ = true;
	loop();
}

void eventm::stop()
{
	running_ = false;
}

void eventm::loop()
{
	while (running_)
	{
		event::shared_ptr_type event = eventQueue_.wait_and_pop();

		if (auto it = eventHandlers_.find(event->type()); it != eventHandlers_.end())
		{
			it->second(event);
		}
	}
}

void eventm::pushEvent(event::shared_ptr_type event)
{
	eventQueue_.push(event);
}

void eventm::registerHandler(eventType type, event_handler_type handler)
{
	if (running_)
	{
		return;
	}
	eventHandlers_[type] = handler;
}
} // namespace tk