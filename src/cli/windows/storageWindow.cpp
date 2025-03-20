#include "cli/windows/storageWindow.h"


#include "cli/core/interface.h"
#include "cli/core/utils.h"
#include "cli/core/window.h"
#include "utils/logger.h"

namespace tk
{
storageWindow::storageWindow(storage* storage, clipboardController* cl, size_t x, size_t y, size_t width, size_t height, const std::string& name)
: borderedWindow(x, y, width, height, name)
, form_(0, 0, width - 2, height - 2)
, storage_(storage)
, cl_(cl)
{
	storage_->setRoot();
	fillForm();
}

void storageWindow::update()
{
	borderedWindow::clear();

	form_.updateBuffer();
	form_.show(*this);

	showWindow(shared_from_this());
}

void storageWindow::handleInputEvent(event::shared_ptr_type event)
{
	if (event->type() != INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type");
		return;
	}
	form_.showSelected();

	auto input = std::static_pointer_cast<inputEvent>(event);

	switch (input->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			if (form_.selectedIndex() == 0)
			{
				cli::core::getScreen().changeControllerWindow("Menu");
				form_.unshowSelected();
				update();
				cli::core::getScreen().show(cli::core::getConsoleManager());
				pushInputEvent(inputEvent::UNKNOWN);
				break;
			}
			form_.switchUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			form_.switchDown();
		}
		break;
		case inputEvent::ENTER:
		{
			auto selected = form_.getSelected();
			if (selected.starts_with("/"))
			{
				if (selected == "/..")
				{
					storage_->folderUp();
				}
				else
				{
					storage_->folderDown(selected.substr(1));
				}
				fillForm();
			}
			else
			{
				if (cl_)
					cl_->write(selected.substr(1));
			}
		}
		break;
		default: break;
	}

	update();
}

void storageWindow::fillForm()
{
	form_.clear();
	auto folder = storage_->currentFolder();
	if (!storage_->curIsRoot())
	{
		form_.addItem("/..");
	}
	for (auto [folderName, _] : folder->subFolders_)
	{
		form_.addItem("/" + folderName);
	}
	for (auto command : folder->commands_)
	{
		form_.addItem(" " + command);
	}
}
} // namespace tk