#pragma once

#include <map>

#include "core/controller.h"

namespace core
{

class controllerm
{
public:
	void registerController(controller::shared_ptr_t controller);
	void unregisterController(uuids::uuid uuid);
	bool setActiveController(uuids::uuid uuid);
	controller::shared_ptr_t getActiveController() const;
	bool active(uuids::uuid uuid) const;

private:
	using controllers_map_t = std::map<uuids::uuid, controller::shared_ptr_t>;
	controllers_map_t controllers_;
	controller::shared_ptr_t activeController_;
};

} // namespace core