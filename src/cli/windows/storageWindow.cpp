#include "cli/windows/storageWindow.h"

#include "config/config.h"

#include "cli/core/events.h"
#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/core/window.h"
#include "cli/forms/hintsForm.h"
#include "os/interface.h"
#include "utils/logger.h"


static const tk::hintsForm::preset_name_type selectionPresetName = "storageSelectionMode";
static const tk::hintsForm::preset_name_type inputPresetName = "storageInputMode";

namespace tk
{
storageWindow::storageWindow(storage::shared_ptr_type storage, cache::shared_ptr_t cache, const std::string& name)
: borderedWindow(name)
, storage_(storage)
, cache_(cache)
{
	selectionForm_.setRelativeSize({ 1, 1 });
	selectionForm_.updateSize(*this);

	LOG_DBG("Storage Window ctor");
	hintsForm_.addPreset(selectionPresetName, config::instance().hintsPreset(selectionPresetName));
	hintsForm_.addPreset(inputPresetName, config::instance().hintsPreset(inputPresetName));
	hintsForm_.applyPreset(selectionPresetName);
	storage_->setRoot();

	LOG_DBG("Filling selection form");
	fillSelectionForm();
}

void storageWindow::update()
{
	updateSize();

	if (inputMode_)
	{
		inputForm_.show(*this);
	}
	else
	{
		selectionForm_.show(*this);
	}

	// hintsForm_.show(*this);
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
		}
		break;
		case inputEvent::ENTER:
		{
			if (event->shiftPressed())
			{
				inputForm_.shiftEnter();
				break;
			}

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
					storage_->renameFolder(tempOldUuid_, userInput[0]);
				}
				break;
				case COMMAND_EDITING:
				{
					storage_->editCommand(tempOldUuid_, userInput[0]);
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
			selectionForm_.switchUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			selectionForm_.switchDown();
		}
		break;
		case inputEvent::ARROW_LEFT:
		{
			cli::core::getScreen().changeControllerWindow(cli::core::getScreen().findLeftNeighbour(uuid()));
		}
		case inputEvent::ENTER:
		{
			auto selected = selectionForm_.getSelected();

			if (selected.content.starts_with("/"))
			{
				if (selected.content == "/..")
				{
					storage_->folderUp();
				}
				else
				{
					storage_->folderDown(selected.uuid);
				}
				fillSelectionForm();
				pushInputEvent(inputEvent::UNSPECIFIED);
			}
			else
			{
				auto content = storage_->findCommand(selected.uuid)->content;
				os::writeToClipboard(content);

				cache_->pushFront(content);
				if (config::instance().closeOnChoice())
				{
					pushExitEvent();
				}
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
			if (selected.content == "/..")
			{
				break;
			}
			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);
			if (selected.content.starts_with("/"))
			{
				inputModeType_ = FOLDER_EDITING;
			}
			else
			{
				inputModeType_ = COMMAND_EDITING;
			}
			tempOldInput_ = selected.content.substr(1);
			tempOldUuid_ = selected.uuid;
			inputForm_.setInput({ tempOldInput_ });
			pushInputEvent(inputEvent::UNSPECIFIED);
		}
		break;
		case inputEvent::DELETE_KEY:
		{
			auto selected = selectionForm_.getSelected();
			if (selected.content.starts_with("/"))
			{
				if (selected.content != "/..")
				{
					storage_->deleteFolder(selected.uuid);
				}
			}
			else
			{
				storage_->deleteCommand(selected.uuid);
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
		selectionForm_.addItem({ "/.." });
	}
	for (auto [_, subFolder] : folder->subFolders_)
	{
		selectionForm_.addItem({ "/" + subFolder->name_, subFolder->uuid_ });
	}
	for (auto command : folder->commands_)
	{
		selectionForm_.addItem({ " " + command->content, command->uuid });
	}
}
} // namespace tk