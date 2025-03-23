#include "cli/windows/storageWindow.h"

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
storageWindow::storageWindow(storage::shared_ptr_type storage, clipboardController::shared_ptr_type clc, cache::shared_ptr_type cache, size_t x, size_t y,
	size_t width, size_t height, const std::string& name)
: borderedWindow(x, y, width, height, name)
, selectionForm_(0, 0, width - 2, height - 3)
, inputForm_(0, 0, width - 2, height - 3, true)
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

void storageWindow::update()
{
	borderedWindow::clear();

	if (inputMode_)
	{
		inputForm_.updateBuffer();
		inputForm_.show(*this);
	}
	else
	{
		selectionForm_.updateBuffer();
		selectionForm_.show(*this);
	}

	hintsForm_.show(*this);
}

void storageWindow::handleInputEvent(event::shared_ptr_type event)
{
	if (event->type() != INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type");
		return;
	}

	if (inputMode_)
	{
		handleInputEventInInputMode(static_pointer_cast<inputEvent>(event));
	}
	else
	{
		handleInputEventInSelectionMode(static_pointer_cast<inputEvent>(event));
	}

	update();
	showWindow(shared_from_this());
}

void storageWindow::handleInputEventInInputMode(inputEvent::shared_ptr_type event)
{
	switch (event->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			inputForm_.moveCursorUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			inputForm_.moveCursorDown();
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
		}
		break;
		case inputEvent::KEY_PRESSED:
		{
			auto ch = *(event->key());
			inputForm_.keyPressed(ch);
		}
		break;
		case inputEvent::SHIFT_ENTER:
		{
			inputForm_.shiftEnter();
		}
		break;
		case inputEvent::ENTER:
		{
			auto userInput = inputForm_.getInput();
			if (userInput.empty() || userInput[0].empty())
			{
				break;
			}
			switch (inputModeType_)
			{
				case FOLDER_CREATING:
				{
					storage_->addFolder(userInput[0]);
				}
				break;
				case COMMAND_CREATING:
				{
					storage_->addCommand(userInput[0]);
				}
				break;
				case FOLDER_EDITING:
				{
					storage_->renameFolder(tempOldInput_, userInput[0]);
				}
				break;
				case COMMAND_EDITING:
				{
					storage_->editCommand(tempOldInput_, userInput[0]);
				}
				break;
				default: break;
			}

			inputForm_.clear();
			inputMode_ = false;
			hintsForm_.applyPreset(selectionPresetName);
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		default: break;
	}
}

void storageWindow::handleInputEventInSelectionMode(inputEvent::shared_ptr_type event)
{
	selectionForm_.showSelected();

	switch (event->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			if (selectionForm_.selectedIndex() == 0)
			{
				cli::core::getScreen().changeControllerWindow("Menu");
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
			auto content = selected.substr(1);
			if (selected.starts_with("/"))
			{
				if (selected == "/..")
				{
					storage_->folderUp();
				}
				else
				{
					storage_->folderDown(content);
				}
				fillSelectionForm();
				pushInputEvent(inputEvent::UNSPECIFIED);
			}
			else
			{
				if (clc_)
					clc_->write(content);
				cache_->addItem(content);
				pushExitEvent();
			}
		}
		break;
		case inputEvent::F1: // adding command
		{
			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);
			inputModeType_ = COMMAND_CREATING;
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::F2: // adding folder
		{
			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);
			inputModeType_ = FOLDER_CREATING;
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::F3: // edit folder/command
		{
			auto selected = selectionForm_.getSelected();
			if (selected == "/..")
			{
				break;
			}
			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);
			if (selected.starts_with("/"))
			{
				inputModeType_ = FOLDER_EDITING;
			}
			else
			{
				inputModeType_ = COMMAND_EDITING;
			}
			tempOldInput_ = selected.substr(1);
			inputForm_.setInput({ tempOldInput_ });
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::DELETE_KEY:
		{
			auto selected = selectionForm_.getSelected();
			if (selected.starts_with("/"))
			{
				if (selected != "/..")
				{
					storage_->deleteFolder(selected.substr(1));
				}
			}
			else
			{
				storage_->deleteCommand(selected.substr(1));
			}
			fillSelectionForm();
			pushInputEvent(inputEvent::UNSPECIFIED);
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

void storageWindow::fillSelectionForm()
{
	selectionForm_.clear();
	auto folder = storage_->currentFolder();
	if (!storage_->curIsRoot())
	{
		selectionForm_.addItem("/..");
	}
	for (auto [folderName, _] : folder->subFolders_)
	{
		selectionForm_.addItem("/" + folderName);
	}
	for (auto command : folder->commands_)
	{
		selectionForm_.addItem(" " + command);
	}
}
} // namespace tk