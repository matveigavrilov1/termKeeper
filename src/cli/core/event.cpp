#include "cli/core/event.h"

namespace tk
{
event::event(unsigned type)
: type_(type)
{ }

unsigned event::type()
{
	return type_;
}
} // namespace tk