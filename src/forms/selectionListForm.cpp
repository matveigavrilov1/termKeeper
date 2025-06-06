#include "forms/selectionListForm.h"

#include "utils/logger.h"

namespace forms
{

selectionListForm::selectionListForm(bool horizontal, const item_list_t& items)
: items_(items)
, horizontal_(horizontal)
, offset_(0)
{
	LOG_DBG("Creating selectionListForm: horizontal=" << horizontal << ", items count=" << items.size());
}

void selectionListForm::show(core::window& wnd)
{
	form::show(wnd);

	LOG_DBG("Showing selectionListForm at position: (" << x() << ", " << y() << "), size: " << width() << "x" << height());

	size_t startX = x(), startY = y();
	size_t edgeX = x() + width(), edgeY = y() + height() ;
	size_t index = offset_;

	LOG_DBG("Starting display loop. edgeX=" << edgeX << ", edgeY=" << edgeY << ", offset=" << offset_ << ", items count=" << items_.size());

	while (startY < edgeY && startX < edgeX && index < items_.size())
	{
		const auto& item = items_[index];
		LOG_DBG("Displaying item[" << index << "]: '" << item.content << "' at (" << startX << ", " << startY << "), selected=" << (index == selectedIndex_));

		showItem(startX, startY, items_[index].content, wnd, index == selectedIndex_);

		if (horizontal_)
		{
			startX += item.content.size() + 1;
			LOG_DBG("Horizontal mode. New startX=" << startX);
			if (startX >= edgeX)
			{
				LOG_DBG("Reached edgeX, breaking loop");
				break;
			}
		}
		else
		{
			auto lines = linesNeeded(item.content);
			LOG_DBG("Vertical mode. Lines needed=" << lines);

			auto nextY = startY + lines;
			if (nextY > edgeY)
			{
				LOG_DBG("Reached edgeY, breaking loop");
				break;
			}
			startY = nextY;
			LOG_DBG("New startY=" << startY);
		}
		++index;
	}

	if (horizontal_)
	{
		LOG_DBG("Horizontal mode cleanup. Incrementing startY from " << startY);
		startY++;
	}

	LOG_DBG("Clearing remaining space from (" << x() << ", " << startY << ") to (" << edgeX << ", " << edgeY << ")");
	while (startY < edgeY)
	{
		startX = x();
		while (startX < edgeX)
		{
			wnd.setContentChar({ startX++, startY }, { ' ' });
		}
		++startY;
	}
}

void selectionListForm::addItem(const item_t& item)
{
	LOG_DBG("Adding item: '" << item.content << "'");
	items_.push_back(item);
}

void selectionListForm::removeItem(const item_t& item)
{
	LOG_DBG("Attempting to remove item: '" << item.content << "'");
	// Реализация удаления должна быть добавлена
}

selectionListForm::item_t selectionListForm::getSelected()
{
	LOG_DBG("Getting selected item. selectedIndex=" << selectedIndex_ << ", items count=" << items_.size());
	if (items_.empty() || selectedIndex_ >= items_.size())
	{
		LOG_DBG("No selected item, returning empty");
		return {};
	}

	LOG_DBG("Returning item: '" << items_[selectedIndex_].content << "'");
	return items_[selectedIndex_];
}

size_t selectionListForm::selectedIndex()
{
	LOG_DBG("Getting selected index: " << selectedIndex_);
	return selectedIndex_;
}

void selectionListForm::switchUp()
{
	LOG_DBG("Switching up. Current selectedIndex=" << selectedIndex_ << ", items count=" << items_.size());
	if (items_.empty())
	{
		LOG_DBG("Items empty, nothing to do");
		return;
	}

	if (selectedIndex_ == 0)
	{
		selectedIndex_ = items_.size() - 1;
		LOG_DBG("Wrapped to end, new selectedIndex=" << selectedIndex_);
	}
	else
	{
		selectedIndex_ -= 1;
		LOG_DBG("Decremented selectedIndex to " << selectedIndex_);
	}
	adjustOffset();
}

void selectionListForm::switchDown()
{
	LOG_DBG("Switching down. Current selectedIndex=" << selectedIndex_ << ", items count=" << items_.size());
	if (items_.empty())
	{
		LOG_DBG("Items empty, nothing to do");
		return;
	}

	if (selectedIndex_ == items_.size() - 1)
	{
		selectedIndex_ = 0;
		LOG_DBG("Wrapped to beginning, new selectedIndex=" << selectedIndex_);
	}
	else
	{
		selectedIndex_ += 1;
		LOG_DBG("Incremented selectedIndex to " << selectedIndex_);
	}
	adjustOffset();
}

void selectionListForm::showSelected()
{
	LOG_DBG("Enabling selected item highlighting");
	showSelected_ = true;
}

void selectionListForm::unshowSelected()
{
	LOG_DBG("Disabling selected item highlighting");
	showSelected_ = false;
}

void selectionListForm::clear()
{
	LOG_DBG("Clearing form. Previous items count=" << items_.size());
	items_.clear();
	selectedIndex_ = 0;
	showSelected_ = false;
	offset_ = 0;
}

bool selectionListForm::empty()
{
	LOG_DBG("Checking if empty: " << (items_.empty() ? "true" : "false"));
	return items_.empty();
}

size_t selectionListForm::linesNeeded(const std::string& str)
{
	if (!width())
	{
		LOG_DBG("Width is 0, returning 1 line for string: '" << str << "'");
		return 1;
	}
	size_t lines = (str.size() + width() - 1) / width();
	LOG_DBG("String '" << str << "' needs " << lines << " lines (width=" << width() << ")");
	return lines;
}

void selectionListForm::showItem(size_t startX, size_t startY, const std::string& str, core::window& wnd, bool selected)
{
	LOG_DBG("Showing item '" << str << "' at (" << startX << ", " << startY << "), selected=" << selected);
	size_t edgeX = pos_.x + width(), edgeY = pos_.y + height();
	auto tmp = startX;

	LOG_DBG("Edge coordinates: edgeX=" << edgeX << ", edgeY=" << edgeY);
	LOG_DBG("Selected highlighting: " << (showSelected_ ? "enabled" : "disabled"));

	for (auto ch : str)
	{
		if (startX >= edgeX || startY >= edgeY)
		{
			LOG_DBG("Reached edge at (" << startX << ", " << startY << "), breaking loop");
			break;
		}

		auto bgColor = (selected && showSelected_) ? os::console::color::CONSOLE_COLOR_WHITE : os::console::color::CONSOLE_COLOR_BLACK;
		auto txtColor = (selected && showSelected_) ? os::console::color::CONSOLE_COLOR_BLACK : os::console::color::CONSOLE_COLOR_WHITE;

		LOG_DBG("Setting char '" << ch << "' at (" << startX << ", " << startY << "), bgColor=" << bgColor << ", txtColor=" << txtColor);
		wnd.setContentChar({ startX++, startY }, { static_cast<unsigned char>(ch), bgColor, txtColor });

		if (startX >= edgeX)
		{
			if (horizontal_)
			{
				LOG_DBG("Horizontal mode and reached edgeX, returning");
				return;
			}
			LOG_DBG("Wrapping to next line");
			startY++;
			startX = tmp;
		}
	}

	LOG_DBG("Filling remaining space with spaces from (" << startX << ", " << startY << ")");
	while (startX < edgeX && startY < edgeY)
	{
		wnd.setContentChar({ startX++, startY }, { ' ' });
	}
}

void selectionListForm::adjustOffset()
{
	LOG_DBG("Adjusting offset. Current offset=" << offset_ << ", selectedIndex=" << selectedIndex_ << ", items count=" << items_.size());

	if (items_.empty())
	{
		LOG_DBG("Items empty, nothing to adjust");
		return;
	}

	if (horizontal_)
	{
		LOG_DBG("Horizontal mode, setting offset to 0");
		offset_ = 0;
		return;
	}

	size_t visibleItems = 0;
	size_t y = pos_.y;
	size_t edgeY_ = pos_.y + height();

	LOG_DBG("Calculating visible items from offset " << offset_ << " within height " << height());
	for (size_t i = offset_; i < items_.size(); ++i)
	{
		size_t needed = linesNeeded(items_[i].content);
		LOG_DBG("Item[" << i << "] needs " << needed << " lines");

		if (y + needed > edgeY_)
		{
			LOG_DBG("Exceeds edgeY " << edgeY_ << ", breaking");
			break;
		}
		y += needed;
		visibleItems++;
		LOG_DBG("Added item[" << i << "], new y=" << y << ", visibleItems=" << visibleItems);
	}

	if (selectedIndex_ < offset_)
	{
		LOG_DBG("Selected index " << selectedIndex_ << " is before offset " << offset_ << ", adjusting offset");
		offset_ = selectedIndex_;
	}
	else if (selectedIndex_ >= offset_ + visibleItems)
	{
		LOG_DBG("Selected index " << selectedIndex_ << " is beyond visible items (offset=" << offset_ << ", visible=" << visibleItems << "), recalculating offset");
		size_t newOffset = selectedIndex_;
		y = pos_.y;

		while (newOffset > 0)
		{
			size_t needed = linesNeeded(items_[newOffset].content);
			LOG_DBG("Item[" << newOffset << "] needs " << needed << " lines");

			if (y + needed > edgeY_)
			{
				LOG_DBG("Exceeds edgeY " << edgeY_ << ", breaking");
				break;
			}
			y += needed;
			newOffset--;
			LOG_DBG("New offset candidate=" << newOffset << ", y=" << y);
		}

		offset_ = newOffset;
		if (y + linesNeeded(items_[selectedIndex_].content) > edgeY_)
		{
			LOG_DBG("Final adjustment needed, incrementing offset");
			offset_++;
		}
	}

	LOG_DBG("Final offset=" << offset_);
}

} // namespace forms