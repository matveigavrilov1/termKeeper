#include "cli/core/event.h"

namespace tk
{
event::event(eventType type)
: type_(type)
{ }

event::event(eventType type, eventData::shared_ptr_type data)
: type_(type)
, data_(data)
{ }

eventType event::type()
{
	return type_;
}

eventData::shared_ptr_type event::data()
{
	return data_;
}

} // namespace tk