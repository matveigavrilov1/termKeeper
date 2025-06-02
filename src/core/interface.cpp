#include "core/interface.h"

#include <stdexcept>

#include "core/controllerm.h"
#include "core/eventm.h"
#include "utils/logger.h"

namespace core
{

bool isInitialized_ { false };

std::optional<class controllerm> controllerm_ { std::nullopt };
std::optional<class eventm> eventm_ { std::nullopt };
std::optional<class screen> screen_ { std::nullopt };

void init()
{
	if (isInitialized_)
		return;

	eventm_.emplace();
	screen_.emplace();
	controllerm_.emplace();

	isInitialized_ = true;
	LOG_INF("Core initialized");
}

class eventm& eventm()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");

	return *eventm_;
}

class screen& screen()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");

	return *screen_;
}

class controllerm& controllerm()
{
	if (!isInitialized_)
		throw std::runtime_error("Core is not initialized");

	return *controllerm_;
}

} // namespace core