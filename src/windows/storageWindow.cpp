#include "windows/storageWindow.h"

#include "config/config.h"

#include "core/events.h"
#include "core/interface.h"
#include "core/utils.h"
#include "core/window.h"
#include "forms/hintsForm.h"
#include "os/interface.h"
#include "utils/logger.h"


static const forms::hintsForm::preset_name_t selectionPresetName = "storageSelectionMode";
static const forms::hintsForm::preset_name_t inputPresetName = "storageInputMode";

namespace wndws
{
storageWindow::storageWindow(data::storage::shared_ptr_t storage, data::cache::shared_ptr_t cache, const std::string& name)
: borderedWindow(name)
, storage_(storage)
, cache_(cache)
{
	LOG_DBG("Constructing storageWindow: " << name);

	selectionForm_.setRelativeSize({ 1, 1 });
	selectionForm_.updateSize(*this);
	LOG_DBG("Selection form size set to relative {1,1}");

	LOG_DBG("Adding hint presets");
	hintsForm_.addPreset(selectionPresetName, conf::config::instance().hintsPreset(selectionPresetName));
	hintsForm_.addPreset(inputPresetName, conf::config::instance().hintsPreset(inputPresetName));
	hintsForm_.applyPreset(selectionPresetName);
	LOG_DBG("Applied selection preset: " << selectionPresetName);

	storage_->setRoot();
	LOG_DBG("Storage root set");

	LOG_DBG("Filling selection form with initial content");
	fillSelectionForm();
}

void storageWindow::update()
{
	LOG_DBG("Updating storageWindow");
	updateSize();

	if (inputMode_)
	{
		LOG_DBG("In input mode, showing input form");
		inputForm_.show(*this);
	}
	else
	{
		LOG_DBG("In selection mode, showing selection form");
		selectionForm_.show(*this);
	}

	// hintsForm_.show(*this);
}

void storageWindow::handleInputEvent(core::event::shared_ptr_t event)
{
	LOG_DBG("Handling input event in storageWindow");

	if (event->type() != core::INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type: " << event->type());
		return;
	}

	if (inputMode_)
	{
		LOG_DBG("Processing event in input mode");
		handleInputEventInInputMode(static_pointer_cast<core::inputEvent>(event));
	}
	else
	{
		LOG_DBG("Processing event in selection mode");
		handleInputEventInSelectionMode(static_pointer_cast<core::inputEvent>(event));
	}

	update();
	LOG_DBG("Showing updated window");
	showWindow(shared_from_this());
}

void storageWindow::handleInputEventInInputMode(core::inputEvent::shared_ptr_type event)
{
	LOG_DBG("Handling input event in input mode. Type: " << event->inputType());

	switch (event->inputType())
	{
		case core::inputEvent::ARROW_UP:
			LOG_DBG("Moving cursor up in input form");
			inputForm_.moveCursorUp();
			break;

		case core::inputEvent::ARROW_DOWN:
			LOG_DBG("Moving cursor down in input form");
			inputForm_.moveCursorDown();
			break;

		case core::inputEvent::ARROW_LEFT:
			LOG_DBG("Moving cursor left in input form");
			inputForm_.moveCursorLeft();
			break;

		case core::inputEvent::ARROW_RIGHT:
			LOG_DBG("Moving cursor right in input form");
			inputForm_.moveCursorRight();
			break;

		case core::inputEvent::BACKSPACE:
			LOG_DBG("Processing backspace in input form");
			inputForm_.backspace();
			break;

		case core::inputEvent::HOME:
			LOG_DBG("Moving cursor to home position");
			inputForm_.home();
			break;

		case core::inputEvent::END:
			LOG_DBG("Moving cursor to end position");
			inputForm_.end();
			break;

		case core::inputEvent::DELETE_KEY:
			LOG_DBG("Deleting character in input form");
			inputForm_.deleteChar();
			break;

		case core::inputEvent::INSERT:
			LOG_DBG("Toggling insert mode");
			inputForm_.toggleInsertMode();
			break;

		case core::inputEvent::KEY_PRESSED:
		{
			auto ch = *(event->key());
			LOG_DBG("Key pressed: " << ch << " (0x" << std::hex << (int)ch << ")");
			inputForm_.keyPressed(ch);
			break;
		}

		case core::inputEvent::ENTER:
		{
			LOG_DBG("Enter pressed in input mode");

			if (event->shiftPressed())
			{
				LOG_DBG("Shift+Enter pressed, adding new line");
				inputForm_.shiftEnter();
				break;
			}

			auto userInput = inputForm_.getInput();
			LOG_DBG("User input: " << (userInput.empty() ? "<empty>" : userInput[0]));

			if (userInput.empty() || userInput[0].empty())
			{
				LOG_DBG("Empty input, ignoring");
				break;
			}

			switch (inputModeType_)
			{
				case FOLDER_CREATING:
					LOG_DBG("Creating new folder: " << userInput[0]);
					storage_->addFolder(userInput[0]);
					break;

				case COMMAND_CREATING:
					LOG_DBG("Creating new command: " << userInput[0]);
					storage_->addCommand(userInput[0]);
					break;

				case FOLDER_EDITING:
					LOG_DBG("Renaming folder from " << tempOldInput_ << " to " << userInput[0]);
					storage_->renameFolder(tempOldUuid_, userInput[0]);
					break;

				case COMMAND_EDITING:
					LOG_DBG("Editing command from " << tempOldInput_ << " to " << userInput[0]);
					storage_->editCommand(tempOldUuid_, userInput[0]);
					break;

				default: LOG_DBG("Unknown input mode type: " << inputModeType_); break;
			}

			inputForm_.clear();
			inputMode_ = false;
			LOG_DBG("Exiting input mode, applying selection preset");
			hintsForm_.applyPreset(selectionPresetName);
			pushInputEvent(core::inputEvent::UNSPECIFIED);
			break;
		}

		default: LOG_DBG("Unhandled input event type in input mode: " << event->inputType()); break;
	}
}

void storageWindow::handleInputEventInSelectionMode(core::inputEvent::shared_ptr_type event)
{
	LOG_DBG("Handling input event in selection mode. Type: " << event->inputType());
	selectionForm_.showSelected();

	switch (event->inputType())
	{
		case core::inputEvent::ARROW_UP:
			LOG_DBG("Moving selection up");
			selectionForm_.switchUp();
			break;

		case core::inputEvent::ARROW_DOWN:
			LOG_DBG("Moving selection down");
			selectionForm_.switchDown();
			break;

		case core::inputEvent::ARROW_LEFT:
			LOG_DBG("Changing to left neighbour window");
			selectionForm_.unshowSelected();
			selectionForm_.show(*this);
			showWindow(shared_from_this());
			core::core::getScreen().changeControllerWindow(core::core::getScreen().findLeftNeighbour(uuid()));
			break;

		case core::inputEvent::ENTER:
		{
			auto selected = selectionForm_.getSelected();
			LOG_DBG("Enter pressed on item: " << selected.content << " (UUID: " << selected.uuid << ")");

			if (selected.content.starts_with("/"))
			{
				if (selected.content == "/..")
				{
					LOG_DBG("Navigating to parent folder");
					storage_->folderUp();
				}
				else
				{
					LOG_DBG("Navigating to subfolder: " << selected.content);
					storage_->folderDown(selected.uuid);
				}
				fillSelectionForm();
				pushInputEvent(core::inputEvent::UNSPECIFIED);
			}
			else
			{
				LOG_DBG("Selected command, writing to uipboard");
				auto command = storage_->findCommand(selected.uuid);
				if (!command)
				{
					LOG_ERR("Command not found with UUID: " << selected.uuid);
					break;
				}

				os::writeToClipboard(command->content);
				LOG_DBG("Command added to cache: " << command->content);
				cache_->pushFront(command->content);

				if (conf::config::instance().closeOnChoice())
				{
					LOG_DBG("closeOnChoice enabled, pushing exit event");
					core::pushExitEvent();
				}
			}
			break;
		}

		case core::inputEvent::F1: // adding command
			LOG_DBG("F1 pressed - entering command creation mode");
			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);
			inputModeType_ = COMMAND_CREATING;
			pushInputEvent(core::inputEvent::UNSPECIFIED);
			break;

		case core::inputEvent::F2: // adding folder
			LOG_DBG("F2 pressed - entering folder creation mode");
			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);
			inputModeType_ = FOLDER_CREATING;
			pushInputEvent(core::inputEvent::UNSPECIFIED);
			break;

		case core::inputEvent::F3: // edit folder/command
		{
			auto selected = selectionForm_.getSelected();
			LOG_DBG("F3 pressed on item: " << selected.content);

			if (selected.content == "/..")
			{
				LOG_DBG("Cannot edit parent folder marker");
				break;
			}

			inputMode_ = true;
			hintsForm_.applyPreset(inputPresetName);

			if (selected.content.starts_with("/"))
			{
				LOG_DBG("Editing folder: " << selected.content);
				inputModeType_ = FOLDER_EDITING;
			}
			else
			{
				LOG_DBG("Editing command: " << selected.content);
				inputModeType_ = COMMAND_EDITING;
			}

			tempOldInput_ = selected.content.substr(1);
			tempOldUuid_ = selected.uuid;
			LOG_DBG("Setting initial input to: " << tempOldInput_);
			inputForm_.setInput({ tempOldInput_ });
			pushInputEvent(core::inputEvent::UNSPECIFIED);
			break;
		}

		case core::inputEvent::DELETE_KEY:
		{
			auto selected = selectionForm_.getSelected();
			LOG_DBG("Delete pressed on item: " << selected.content);

			if (selected.content.starts_with("/"))
			{
				if (selected.content != "/..")
				{
					LOG_DBG("Deleting folder: " << selected.content);
					storage_->deleteFolder(selected.uuid);
				}
				else
				{
					LOG_DBG("Cannot delete parent folder marker");
				}
			}
			else
			{
				LOG_DBG("Deleting command: " << selected.content);
				storage_->deleteCommand(selected.uuid);
			}

			fillSelectionForm();
			pushInputEvent(core::inputEvent::UNSPECIFIED);
			break;
		}

		case core::inputEvent::UNSPECIFIED:
			LOG_DBG("Refresh event received");
			fillSelectionForm();
			selectionForm_.showSelected();
			break;

		default: LOG_DBG("Unhandled input event type in selection mode: " << event->inputType()); break;
	}
}

void storageWindow::fillSelectionForm()
{
	LOG_DBG("Filling selection form with current folder content");
	selectionForm_.clear();

	auto folder = storage_->currentFolder();
	LOG_DBG("Current folder: " << folder->name_);

	if (!storage_->curIsRoot())
	{
		LOG_DBG("Adding parent folder marker");
		selectionForm_.addItem({ "/.." });
	}

	for (auto [_, subFolder] : folder->subFolders_)
	{
		LOG_DBG("Adding subfolder: /" << subFolder->name_ << " (UUID: " << subFolder->uuid_ << ")");
		selectionForm_.addItem({ "/" + subFolder->name_, subFolder->uuid_ });
	}

	for (auto command : folder->commands_)
	{
		LOG_DBG("Adding command: " << command->content << " (UUID: " << command->uuid << ")");
		selectionForm_.addItem({ " " + command->content, command->uuid });
	}

	LOG_DBG("Selection form filled with " << folder->subFolders_.size() << " folders and " << folder->commands_.size() << " commands");
}
} // namespace wndws