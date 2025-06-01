#pragma once

#include <memory>
#include <string>

#include "core/events.h"

#include "windows/borderedWindow.h"
#include "forms/hintsForm.h"
#include "forms/inputForm.h"
#include "forms/selectionListForm.h"

#include "storage/cache.h"
#include "storage/storage.h"

namespace tk
{
class storageWindow final
: public borderedWindow
, public std::enable_shared_from_this<storageWindow>
{
public:
	storageWindow(storage::shared_ptr_type storage, cache::shared_ptr_t cache, const std::string& name = "Storage");

	void update() override;
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
	uuids::uuid tempOldUuid_;

	storage::shared_ptr_type storage_;
	cache::shared_ptr_t cache_;
};
} // namespace tk