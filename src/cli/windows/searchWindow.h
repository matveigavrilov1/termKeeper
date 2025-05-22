#pragma once

#include <memory>
#include <string>

#include "cli/core/events.h"
#include "clipboardController/interface.h"

#include "cli/windows/borderedWindow.h"
#include "cli/forms/hintsForm.h"
#include "cli/forms/inputForm.h"
#include "cli/forms/selectionListForm.h"

#include "storage/cache.h"
#include "storage/storage.h"

namespace tk
{
class searchWindow
: public borderedWindow
, public std::enable_shared_from_this<searchWindow>
{
public:
	searchWindow(storage::shared_ptr_type storage, clipboardController::shared_ptr_type clc, cache::shared_ptr_type cache, size_t x, size_t y, size_t width,
		size_t height, const std::string& name = "Search");

	void update() override;
	void handleInputEvent(event::shared_ptr_type event) override;

	void inputFormHandler(inputEvent::shared_ptr_type event);
	void selectionFormHandler(inputEvent::shared_ptr_type event);

private:
	void fillSelectionForm();

private:
	selectionListForm selectionForm_;
	inputForm inputForm_;
	hintsForm hintsForm_;

	enum inputModeType
	{
		FOLDER_CREATING,
		COMMAND_CREATING,
		FOLDER_EDITING,
		COMMAND_EDITING,
		NONE
	};

	bool inputMode_ { true };

	storage::shared_ptr_type storage_;
	cache::shared_ptr_type cache_;
	clipboardController::shared_ptr_type clc_;
};
} // namespace tk