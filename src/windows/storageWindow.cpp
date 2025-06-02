#include "windows/storageWindow.h"

#include "config/config.h"

#include "core/events.h"
#include "core/utils.h"
#include "forms/hintsForm.h"
#include "os/interface.h"
#include "utils/logger.h"


static const forms::hintsForm::preset_name_t selectionPresetName = "storageSelectionMode";
static const forms::hintsForm::preset_name_t inputPresetName = "storageInputMode";

namespace wndws
{
storageWindow::storageWindow(data::storage::shared_ptr_t storage, data::cache::shared_ptr_t cache, const std::string& name)
: controllerWindow(name)
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

void storageWindow::updateBordered()
{
	LOG_DBG("Updating storageWindow");

	if (inputMode_)
	{
		LOG_DBG("In input mode, showing input form");
		inputForm_.show(*this);
	}
	else
	{
		LOG_DBG("In selection mode, showing selection form");
		isThisController() ? selectionForm_.showSelected() : selectionForm_.unshowSelected();
		selectionForm_.show(*this);
	}

	// hintsForm_.show(*this);
}

bool storageWindow::handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.moveCursorUp();
	}
	else
	{
		selectionForm_.switchUp();
	}
	return true;
}

bool storageWindow::handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.moveCursorDown();
	}
	else
	{
		selectionForm_.switchDown();
	}
	return true;
}

bool storageWindow::handleArrowLeftDecorator(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.moveCursorLeft();
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleArrowRightDecorator(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.moveCursorRight();
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleKeyPressed(char ch, const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.keyPressed(ch);
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleEnter(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		if (mods.shift)
		{
			LOG_DBG("Shift+Enter pressed, adding new line");
			inputForm_.shiftEnter();
			return true;
		}

		auto userInput = inputForm_.getInput();
		LOG_DBG("User input: " << (userInput.empty() ? "<empty>" : userInput[0]));

		if (userInput.empty() || userInput[0].empty())
		{
			LOG_DBG("Empty input, ignoring");
			return true;
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
	}
	else
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
				return true;
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
	}
	return true;
}

bool storageWindow::handleBackspace(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.backspace();
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleHome(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.home();
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleEnd(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.end();
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleInsert(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.toggleInsertMode();
	}
	else
	{
		return false;
	}
	return true;
}

bool storageWindow::handleDelete(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		inputForm_.deleteChar();
	}
	else
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
	}
	return true;
}

bool storageWindow::handleF1(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		return false;
	}
	else
	{
		inputMode_ = true;
		inputModeType_ = COMMAND_CREATING;
	}
	return true;
}

bool storageWindow::handleF2(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		return false;
	}
	else
	{
		inputMode_ = true;
		inputModeType_ = FOLDER_CREATING;
	}
	return true;
}

bool storageWindow::handleF3(const core::inputEvent::keyModifiers& mods)
{
	if (inputMode_)
	{
		return false;
	}
	else
	{
		auto selected = selectionForm_.getSelected();
		LOG_DBG("F3 pressed on item: " << selected.content);

		if (selected.content == "/..")
		{
			LOG_DBG("Cannot edit parent folder marker");
			return true;
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
	}
	return true;
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