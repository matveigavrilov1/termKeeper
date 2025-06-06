#pragma once

#include <memory>
#include <string>

#include "core/events.h"

#include "windows/utils/controllerWindow.h"
#include "forms/hintsForm.h"
#include "forms/inputForm.h"
#include "forms/selectionListForm.h"

#include "data/cache.h"
#include "data/storage.h"

namespace wndws
{
class storageWindow final : public controllerWindow
{
public:
	storageWindow(data::storage::shared_ptr_t storage, data::cache::shared_ptr_t cache, const std::string& name = "Storage");

	void updateBordered() override;


protected:
	bool handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowLeftDecorator(const core::inputEvent::keyModifiers& mods) override;
	bool handleArrowRightDecorator(const core::inputEvent::keyModifiers& mods) override;

	bool handleKeyPressed(char ch, const core::inputEvent::keyModifiers& mods) override;
	bool handleEnter(const core::inputEvent::keyModifiers& mods) override;
	bool handleBackspace(const core::inputEvent::keyModifiers& mods) override;

	bool handleHome(const core::inputEvent::keyModifiers& mods) override;
	bool handleEnd(const core::inputEvent::keyModifiers& mods) override;
	bool handleInsert(const core::inputEvent::keyModifiers& mods) override;
	bool handleDelete(const core::inputEvent::keyModifiers& mods) override;

	bool handleF1(const core::inputEvent::keyModifiers& mods) override;
	bool handleF2(const core::inputEvent::keyModifiers& mods) override;
	bool handleF3(const core::inputEvent::keyModifiers& mods) override;

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