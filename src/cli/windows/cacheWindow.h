#pragma once

#include <memory>
#include <string>

#include "cli/forms/selectionListForm.h"
#include "cli/forms/hintsForm.h"
#include "cli/windows/borderedWindow.h"

#include "storage/cache.h"

#include "utils/observer.h"

namespace tk
{
class cacheWindow final
: public borderedWindow
, public std::enable_shared_from_this<cacheWindow>
, public observer
{
public:
	cacheWindow(cache::shared_ptr_t cache, const std::string& name = "Cache");
	void update() override;

	void handleInputEvent(event::shared_ptr_type event) override;

	void update(const std::string&) override;

private:
	void fillForm();

	cache::shared_ptr_t cache_;
	int activeIndex_ { 0 };
	selectionListForm form_;

	hintsForm hintsForm_;
};
} // namespace tk