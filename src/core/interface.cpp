#include "core/interface.h"

#include <stdexcept>

#include "core/eventm.h"
#include "utils/logger.h"

namespace tk
{
namespace ui
{
bool core::isInitialized_ { false };

std::optional<eventm> core::eventm_ { std::nullopt };
std::optional<screen> core::screen_ { std::nullopt };

void core::init()
{
	if (isInitialized_)
		return;

	eventm_.emplace();
	screen_.emplace();

	isInitialized_ = true;
	LOG_INF("Core initialized");
}

eventm& core::getEventManager()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");


	return *eventm_;
}

screen& core::getScreen()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");


	return *screen_;
}
} // namespace ui
} // namespace tk