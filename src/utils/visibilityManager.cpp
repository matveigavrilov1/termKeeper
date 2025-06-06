#include "utils/visibilityManager.h"

#include "os/console.h"

void utils::visibilityManager::hide()
{
	for (auto& callback : callbacks_)
		callback();
	os::console::get()->hide();
}

void utils::visibilityManager::addCallback(on_hide_callback_t callback)
{
	callbacks_.push_back(callback);
}