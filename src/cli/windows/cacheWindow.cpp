#include "cli/windows/cacheWindow.h"

#include <memory>

#include "cli/forms/selectionListForm.h"
#include "config/config.h"

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/windows/borderedWindow.h"
#include "os/interface.h"
#include "utils/logger.h"


static const tk::hintsForm::preset_name_type cachePresetName = "cache";

namespace tk
{
cacheWindow::cacheWindow(cache::shared_ptr_type cache, const std::string& name)
: borderedWindow( name)
, cache_(cache)
{
	hintsForm_.setRelativeSize({1, 1});
	form_.setRelativeSize({1, 1});

	updateSize();

	hintsForm_.addPreset(cachePresetName, config::instance().hintsPreset(cachePresetName));
	hintsForm_.applyPreset(cachePresetName);
	fillForm();
}

void cacheWindow::update()
{
	updateSize();

	form_.show(*this);
	hintsForm_.show(*this);
}

void cacheWindow::handleInputEvent(event::shared_ptr_type event)
{
	if (event->type() != INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type");
		return;
	}
	form_.showSelected();

	auto input = std::static_pointer_cast<inputEvent>(event);

	switch (input->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			if (form_.selectedIndex() == 0)
			{
				cli::core::getScreen().changeControllerWindow("Menu");
				form_.unshowSelected();
				update();
				cli::core::getScreen().show(os::console::get());
				pushInputEvent(inputEvent::UNSPECIFIED);
				break;
			}
			form_.switchUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			form_.switchDown();
		}
		break;
		case inputEvent::ENTER:
		{
			auto selected = form_.getSelected();
			os::writeToClipboard(selected);

			cache_->pushFront(selected);
			if (config::instance().closeOnChoice())
			{
				pushExitEvent();
			}
		}
		break;
		default: break;
	}

	update();
	showWindow(shared_from_this());
	if (form_.empty())
	{
		cli::core::getScreen().changeControllerWindow("Menu");
		pushInputEvent(inputEvent::UNSPECIFIED);
	}
}

void cacheWindow::update(const std::string&)
{
	fillForm();
	form_.show(*this);
	if (cli::core::getScreen().controllerWindow()->name() == name())
	{
		pushInputEvent(inputEvent::UNSPECIFIED);
	}
}

void cacheWindow::fillForm()
{
	form_.clear();
	for (auto command : cache_->getCache())
	{
		form_.addItem(command);
	}
}
} // namespace tk