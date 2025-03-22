#pragma once

#include <memory>
#include <string>

#include "cli/forms/selectionListForm.h"
#include "cli/windows/borderedWindow.h"
#include "clipboardController/interface.h"

#include "storage/cache.h"

#include "utils/observer.h"

namespace tk
{
class cacheWindow
: public borderedWindow
, public std::enable_shared_from_this<cacheWindow>
, public observer
{
public:
	cacheWindow(
		cache::shared_ptr_type, clipboardController::shared_ptr_type clc, size_t x, size_t y, size_t width, size_t height, const std::string& name = "Cache");
	void update() override;

	void handleInputEvent(event::shared_ptr_type event) override;

	void update(const std::string&) override;

private:
	void fillForm();

	cache::shared_ptr_type cache_;
	clipboardController::shared_ptr_type clc_;
	int activeIndex_ { 0 };
	selectionListForm form_;
};
} // namespace tk