#include "cli/windows/cacheWindow.h"

#include "cli/forms/selectionListForm.h"
#include "config/config.h"

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/windows/borderedWindow.h"
#include "utils/logger.h"
#include <memory>

static const tk::hintsForm::preset_name_type cachePresetName = "cache";

namespace tk
{
cacheWindow::cacheWindow(cache::shared_ptr_type cache, clipboardController::shared_ptr_type clc, size_t x, size_t y, size_t width, size_t height, const std::string& name)
: borderedWindow(x, y, width, height, name)
, form_ {
	0,
	0,
	width - 2,
	height - 3,
}
, hintsForm_(0, height - 3, width - 2, 1)
, cache_(cache)
, clc_(clc)
{
	hintsForm_.addPreset(cachePresetName, config::instance().hintsPreset(cachePresetName));
	hintsForm_.applyPreset(cachePresetName);
	hintsForm_.updateBuffer();
	fillForm();
}

void cacheWindow::update()
{
	LOG_DBG("Updating Cache Window");
	borderedWindow::clear();

	form_.updateBuffer();
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
				cli::core::getScreen().show(cli::core::getConsoleManager());
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
			if (clc_)
				clc_->write(selected);
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
	form_.updateBuffer();
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