#pragma once

#include <memory>
#include <string>

#include "forms/selectionListForm.h"
#include "forms/hintsForm.h"
#include "windows/utils/controllerWindow.h"

#include "data/cache.h"

#include "utils/observer.h"

namespace wndws
{
class cacheWindow final
: public controllerWindow
, public utils::observer
{
public:
	cacheWindow(data::cache::shared_ptr_t cache, const std::string& name = "Cache");
	void updateBordered() override;

	void update(const std::string&) override;

protected:
	bool handleArrowUpDecorator(const core::inputEvent::keyModifiers &mods) override;
	bool handleArrowDownDecorator(const core::inputEvent::keyModifiers &mods) override;
	bool handleEnter(const core::inputEvent::keyModifiers &mods) override;

private:
	void fillForm();

	data::cache::shared_ptr_t cache_;
	int activeIndex_ { 0 };
	forms::selectionListForm form_;

	forms::hintsForm hintsForm_;
};
} // namespace wndws