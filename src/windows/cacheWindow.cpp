#include "windows/cacheWindow.h"

#include <memory>

#include "forms/selectionListForm.h"
#include "config/config.h"

#include "core/events.h"
#include "core/interface.h"
#include "core/utils.h"
#include "windows/borderedWindow.h"
#include "os/interface.h"
#include "utils/logger.h"


static const tk::hintsForm::preset_name_type cachePresetName = "cache";

namespace tk
{
cacheWindow::cacheWindow(cache::shared_ptr_t cache, const std::string& name)
: borderedWindow(name)
, cache_(cache)
{
	LOG_DBG("Creating cacheWindow with name: " << name);
	clear();

	hintsForm_.setRelativeSize({ 1, 1 });
	form_.setRelativeSize({ 1, 1 });
	updateSize();

	LOG_DBG("Setting up hints form with preset: " << cachePresetName);
	hintsForm_.addPreset(cachePresetName, config::instance().hintsPreset(cachePresetName));
	hintsForm_.applyPreset(cachePresetName);

	LOG_DBG("Filling form with cache items");
	fillForm();
}

void cacheWindow::update()
{
	LOG_DBG("Updating cacheWindow");
	updateSize();

	LOG_DBG("Showing main form");
	form_.show(*this);
	// hintsForm_.show(*this);
}

void cacheWindow::handleInputEvent(event::shared_ptr_type event)
{
	LOG_DBG("Handling input event in cacheWindow: " << event->type());

	if (event->type() != INPUT_EVENT)
	{
		LOG_ERR("Incorrect event type: " << event->type());
		return;
	}

	LOG_DBG("Showing selected item in form");
	form_.showSelected();

	auto input = std::static_pointer_cast<inputEvent>(event);
	LOG_DBG("Input event type: " << input->inputType());

	switch (input->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			LOG_DBG("Processing ARROW_UP event");
			form_.switchUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			LOG_DBG("Processing ARROW_DOWN event");
			form_.switchDown();
		}
		break;
		case inputEvent::ARROW_LEFT:
		{
			LOG_DBG("Processing ARROW_LEFT event");
			ui::core::getScreen().changeControllerWindow(ui::core::getScreen().findLeftNeighbour(uuid()));
			LOG_DBG("Changed controller window to left neighbour");
		}
		break;
		case inputEvent::ENTER:
		{
			LOG_DBG("Processing ENTER event");
			auto selected = form_.getSelected();
			LOG_DBG("Selected item UUID: " << selected.uuid);

			auto item = cache_->findItem(selected.uuid);
			if (!item)
			{
				LOG_ERR("Item not found in cache with UUID: " << selected.uuid);
				break;
			}

			LOG_DBG("Writing item content to uipboard: " << item->content);
			os::writeToClipboard(item->content);

			LOG_DBG("Moving item to front of cache");
			cache_->pushFront(item->content);

			if (config::instance().closeOnChoice())
			{
				LOG_DBG("Config closeOnChoice is true, pushing exit event");
				pushExitEvent();
			}
		}
		break;
		default: LOG_DBG("Unhandled input event type: " << input->inputType()); break;
	}

	update();
	LOG_DBG("Showing updated window");
	showWindow(shared_from_this());

	if (form_.empty())
	{
		LOG_DBG("Form is empty, changing to left neighbour window");
		ui::core::getScreen().changeControllerWindow(ui::core::getScreen().findLeftNeighbour(uuid()));
	}
}

void cacheWindow::update(const std::string&)
{
	LOG_DBG("Updating cacheWindow with string parameter");
	fillForm();
	form_.show(*this);

	if (ui::core::getScreen().controllerWindow()->name() == name())
	{
		LOG_DBG("This window is controller, pushing unspecified input event");
		pushInputEvent(inputEvent::UNSPECIFIED);
	}
}

void cacheWindow::fillForm()
{
	LOG_DBG("Filling form with cache items");
	form_.clear();

	auto cacheItems = cache_->getCache();
	LOG_DBG("Cache contains " << cacheItems.size() << " items");

	for (auto command : cacheItems)
	{
		LOG_DBG("Adding item to form: " << command->content << " (UUID: " << command->uuid << ")");
		form_.addItem({ command->content, command->uuid });
	}

	LOG_DBG("Form now contains " << (form_.empty() ? "no" : std::to_string(cacheItems.size())) << " items");
}
} // namespace tk