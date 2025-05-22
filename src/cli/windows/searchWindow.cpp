#include "cli/windows/searchWindow.h"

#include "config/config.h"

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/core/window.h"
#include "cli/forms/hintsForm.h"
#include "utils/logger.h"


static const tk::hintsForm::preset_name_type selectionPresetName = "storageSelectionMode";
static const tk::hintsForm::preset_name_type inputPresetName = "storageInputMode";

namespace tk
{
searchWindow::searchWindow(storage::shared_ptr_type storage, clipboardController::shared_ptr_type clc, cache::shared_ptr_type cache, size_t x, size_t y,
	size_t width, size_t height, const std::string& name)
: borderedWindow(x, y, width, height, name)
, selectionForm_(0, 1, width - 2, height - 3)
, inputForm_(0, 0, width - 2, 1, true)
, hintsForm_(0, height - 3, width - 2, 1)
, storage_(storage)
, clc_(clc)
, cache_(cache)
{
	hintsForm_.addPreset(selectionPresetName, config::instance().hintsPreset(selectionPresetName));
	hintsForm_.addPreset(inputPresetName, config::instance().hintsPreset(inputPresetName));
	hintsForm_.applyPreset(selectionPresetName);
	storage_->setRoot();
	fillSelectionForm();
}

void searchWindow::update()
{
	inputForm_.show(*this);
	selectionForm_.show(*this);
	hintsForm_.show(*this);
}

void searchWindow::handleInputEvent(event::shared_ptr_type event)
{
	if (event->type() != INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type");
		return;
	}

	if (inputMode_)
	{
		inputFormHandler(static_pointer_cast<inputEvent>(event));
	}
	else
	{
		selectionFormHandler(static_pointer_cast<inputEvent>(event));
	}

	update();
	showWindow(shared_from_this());
}

void searchWindow::inputFormHandler(inputEvent::shared_ptr_type event)
{
	selectionForm_.unshowSelected();
	inputForm_.showCursor();
	switch (event->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			cli::core::getScreen().changeControllerWindow("Menu");
			inputForm_.unshowCursor();
			selectionForm_.unshowSelected();
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			inputMode_ = false;
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::ARROW_LEFT:
		{
			inputForm_.moveCursorLeft();
		}
		break;
		case inputEvent::ARROW_RIGHT:
		{
			inputForm_.moveCursorRight();
		}
		break;
		case inputEvent::BACKSPACE:
		{
			inputForm_.backspace();
			fillSelectionForm();
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::HOME:
		{
			inputForm_.home();
		}
		break;
		case inputEvent::END:
		{
			inputForm_.end();
		}
		break;
		case inputEvent::DELETE_KEY:
		{
			inputForm_.deleteChar();
		}
		break;
		case inputEvent::INSERT:
		{
			inputForm_.toggleInsertMode();
		}
		break;
		case inputEvent::KEY_PRESSED:
		{
			auto ch = *(event->key());
			inputForm_.keyPressed(ch);
			fillSelectionForm();
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::SHIFT_ENTER:
		{
			inputForm_.shiftEnter();
		}
		break;
		case inputEvent::ENTER:
		{
			inputMode_ = false;
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		default: break;
	}
}

void searchWindow::selectionFormHandler(inputEvent::shared_ptr_type event)
{
	inputForm_.unshowCursor();
	selectionForm_.showSelected();

	switch (event->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			if (selectionForm_.selectedIndex() == 0)
			{
				inputMode_ = true;
				selectionForm_.unshowSelected();
				pushInputEvent(inputEvent::UNSPECIFIED);
				break;
			}
			selectionForm_.switchUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			selectionForm_.switchDown();
		}
		break;
		case inputEvent::ENTER:
		{
			auto selected = selectionForm_.getSelected();
			if (clc_)
				clc_->write(selected);
			cache_->pushFront(selected);
			if (config::instance().closeOnChoice())
			{
				pushExitEvent();
			}
		}
		break;
		case inputEvent::UNSPECIFIED:
		{
			fillSelectionForm();
			selectionForm_.showSelected();
		}
		break;
		default: break;
	}
}

void searchWindow::fillSelectionForm()
{
	selectionForm_.clear();
	auto commands = storage_->search(inputForm_.getInput()[0]);
	for (const auto& cmd : commands)
	{
		selectionForm_.addItem(cmd);
	}
}
} // namespace tk