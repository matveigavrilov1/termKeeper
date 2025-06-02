#pragma once

#include <memory>
#include <string>

#include "core/events.h"

#include "windows/borderedWindow.h"
#include "forms/hintsForm.h"
#include "forms/inputForm.h"
#include "forms/selectionListForm.h"

#include "data/cache.h"
#include "data/storage.h"

namespace wndws
{
class storageWindow final
: public borderedWindow
, public std::enable_shared_from_this<storageWindow>
{
public:
	storageWindow(data::storage::shared_ptr_t storage, data::cache::shared_ptr_t cache, const std::string& name = "Storage");

	void update() override;
	void handleInputEvent(core::event::shared_ptr_t event) override;

	void handleInputEventInInputMode(core::inputEvent::shared_ptr_type event);
	void handleInputEventInSelectionMode(core::inputEvent::shared_ptr_type event);

private:
	void fillSelectionForm();

private:
	forms::selectionListForm selectionForm_;
	forms::inputForm inputForm_;
	forms::hintsForm hintsForm_;

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

	data::storage::shared_ptr_t storage_;
	data::cache::shared_ptr_t cache_;
};
} // namespace wndws