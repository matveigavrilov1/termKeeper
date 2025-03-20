#pragma once

#include <memory>
#include <string>

#include "clipboardController/interface.h"

#include "cli/windows/borderedWindow.h"
#include "cli/forms/selectionListForm.h"

#include "storage/storage.h"

namespace tk
{
class storageWindow
: public borderedWindow
, public std::enable_shared_from_this<storageWindow>
{
public:
	storageWindow(storage* storage, clipboardController* cl, size_t x, size_t y, size_t width, size_t height, const std::string& name = "Storage");

	void update();
	void handleInputEvent(event::shared_ptr_type event) override;

private:
	void fillForm();

private:
	selectionListForm form_;
	storage* storage_;
	clipboardController* cl_;
};
} // namespace tk