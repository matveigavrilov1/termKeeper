#include "core/controllerm.h"

namespace core
{

void controllerm::registerController(controller::shared_ptr_t controller)
{
	if (controller)
	{
		controllers_[controller->uuid()] = controller;
	}
}

void controllerm::unregisterController(uuids::uuid uuid)
{
	auto it = controllers_.find(uuid);
	if (it != controllers_.end())
	{
		if (it->second == activeController_)
		{
			activeController_ = nullptr;
		}
		controllers_.erase(it);
	}
}

bool controllerm::setActiveController(uuids::uuid uuid)
{
	auto it = controllers_.find(uuid);
	if (it != controllers_.end())
	{
		activeController_ = it->second;
		return true;
	}
	return false;
}

controller::shared_ptr_t controllerm::getActiveController() const
{
	return activeController_;
}

bool controllerm::active(uuids::uuid uuid) const
{
	if (!activeController_)
		return false;
	return activeController_->uuid() == uuid;
}
} // namespace core