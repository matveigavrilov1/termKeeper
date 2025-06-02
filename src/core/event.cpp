#include "core/event.h"

namespace core
{
event::event(unsigned type)
: type_(type)
{ }

unsigned event::type()
{
	return type_;
}
} // namespace core