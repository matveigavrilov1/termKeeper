#include "windows/cacheWindow.h"

#include "forms/selectionListForm.h"
#include "config/config.h"

#include "core/events.h"
#include "core/utils.h"
#include "os/interface.h"
#include "utils/logger.h"
#include "windows/utils/controllerWindow.h"


static const forms::hintsForm::preset_name_t cachePresetName = "cache";

namespace wndws
{
cacheWindow::cacheWindow(data::cache::shared_ptr_t cache, const std::string& name)
: controllerWindow(name)
, cache_(cache)
{
	LOG_DBG("Creating cacheWindow with name: " << name);
	clear();

	hintsForm_.setRelativeSize({ 1, 1 });
	form_.setRelativeSize({ 1, 1 });
	updateSize();

	LOG_DBG("Setting up hints form with preset: " << cachePresetName);
	hintsForm_.addPreset(cachePresetName, conf::config::instance().hintsPreset(cachePresetName));
	hintsForm_.applyPreset(cachePresetName);

	LOG_DBG("Filling form with cache items");
	fillForm();
}

void cacheWindow::updateBordered()
{
	LOG_DBG("Updating cacheWindow");
	isThisController() ? form_.showSelected() : form_.unshowSelected();

	LOG_DBG("Showing main form");
	form_.show(*this);
	// hintsForm_.show(*this);
}

void cacheWindow::update(const std::string&)
{
	LOG_DBG("Updating cacheWindow with string parameter");
	fillForm();
	form_.show(*this);
}

bool cacheWindow::handleArrowUpDecorator(const core::inputEvent::keyModifiers& mods)
{
	form_.switchUp();
	return true;
}

bool cacheWindow::handleArrowDownDecorator(const core::inputEvent::keyModifiers& mods)
{
	form_.switchDown();
	return true;
}

bool cacheWindow::handleEnter(const core::inputEvent::keyModifiers& mods)
{
	LOG_DBG("Processing ENTER event");
	auto selected = form_.getSelected();
	LOG_DBG("Selected item UUID: " << selected.uuid);

	auto item = cache_->findItem(selected.uuid);
	if (!item)
	{
		LOG_ERR("Item not found in cache with UUID: " << selected.uuid);
		return false;
	}

	LOG_DBG("Writing item content to uipboard: " << item->content);
	os::writeToClipboard(item->content);

	LOG_DBG("Moving item to front of cache");
	cache_->pushFront(item->content);

	if (conf::config::instance().closeOnChoice())
	{
		LOG_DBG("Config closeOnChoice is true, pushing exit event");
		core::pushExitEvent();
	}
	return true;
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
} // namespace wndws