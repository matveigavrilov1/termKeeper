#pragma once

#include <memory>
#include <string>

#include "forms/selectionListForm.h"
#include "forms/hintsForm.h"
#include "windows/borderedWindow.h"

#include "data/cache.h"

#include "utils/observer.h"

namespace wndws
{
class cacheWindow final
: public borderedWindow
, public std::enable_shared_from_this<cacheWindow>
, public utils::observer
{
public:
	cacheWindow(data::cache::shared_ptr_t cache, const std::string& name = "Cache");
	void update() override;

	void handleInputEvent(core::event::shared_ptr_t event) override;

	void update(const std::string&) override;

private:
	void fillForm();

	data::cache::shared_ptr_t cache_;
	int activeIndex_ { 0 };
	forms::selectionListForm form_;

	forms::hintsForm hintsForm_;
};
} // namespace wndws