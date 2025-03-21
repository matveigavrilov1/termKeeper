#pragma once

#include <memory>
#include <string>

#include "cli/core/events.h"
#include "clipboardController/interface.h"

#include "cli/windows/borderedWindow.h"
#include "cli/forms/hintsForm.h"
#include "cli/forms/inputForm.h"
#include "cli/forms/selectionListForm.h"

#include "storage/storage.h"

namespace tk
{
class storageWindow
: public borderedWindow
, public std::enable_shared_from_this<storageWindow>
{
public:
	storageWindow(storage::shared_ptr_type storage, clipboardController::shared_ptr_type clc, size_t x, size_t y, size_t width, size_t height, const std::string& name = "Storage");

	void update();
	void handleInputEvent(event::shared_ptr_type event) override;

	void handleInputEventInInputMode(inputEvent::shared_ptr_type event);
	void handleInputEventInSelectionMode(inputEvent::shared_ptr_type event);

private:
	void fillSelectionForm();

private:
	selectionListForm selectionForm_;
	inputForm inputForm_;
	hintsForm hintsForm_;

	bool inputMode_ { false };

	enum inputModeType
	{
		FOLDER_CREATING,
		COMMAND_CREATING,
		FOLDER_EDITING,
		COMMAND_EDITING,
		NONE
	};

	inputModeType inputModeType_ { NONE };
	std::string tempOldInput_;

	storage::shared_ptr_type storage_;
	clipboardController::shared_ptr_type clc_;
};
} // namespace tk