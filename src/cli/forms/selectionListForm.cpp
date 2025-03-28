#include "cli/forms/selectionListForm.h"
#include "utils/logger.h"

namespace tk
{

selectionListForm::selectionListForm(size_t x, size_t y, size_t width, size_t height, bool horizontal, const item_list_type& items)
: form(x, y, width, height)
, items_(items)
, horizontal_(horizontal)
{ }

void selectionListForm::show(window& wnd)
{
	size_t x = x_, y = y_;
	size_t edgeX_ = x_ + width_, edgeY_ = y_ + height_;
	auto index = 0;
	while (y != edgeY_ && x != edgeX_ && index < items_.size())
	{
		const auto& item = items_[index];
		auto nextY = y + linesNeeded(item);

		if (nextY > edgeY_)
		{
			break;
		}

		showItem(x, y, items_[index], wnd, index == selectedIndex_);

		if (horizontal_)
		{
			x += item.size() + 1;
		}
		else
		{
			y = nextY;
		}
		++index;
	}
}

void selectionListForm::addItem(const item_type& item)
{
	items_.push_back(item);
}

void selectionListForm::removeItem(const item_type& item) { }

selectionListForm::item_type selectionListForm::getSelected()
{
	if (items_.empty() || selectedIndex_ >= items_.size())
	{
		return {};
	}
	return items_[selectedIndex_];
}

size_t selectionListForm::selectedIndex()
{
	return selectedIndex_;
}

void selectionListForm::switchUp()
{
	if (items_.empty())
	{
		return;
	}
	if (selectedIndex_ == 0)
	{
		selectedIndex_ = items_.size() - 1;
	}
	else
	{
		selectedIndex_ -= 1;
	}
}

void selectionListForm::switchDown()
{
	if (items_.empty())
	{
		return;
	}
	if (selectedIndex_ == items_.size() - 1)
	{
		selectedIndex_ = 0;
	}
	else
	{
		selectedIndex_ += 1;
	}
}

void selectionListForm::showSelected()
{
	showSelected_ = true;
}

void selectionListForm::unshowSelected()
{
	showSelected_ = false;
}

void selectionListForm::clear()
{
	items_.clear();
	selectedIndex_ = 0;
	showSelected_ = false;
}

bool selectionListForm::empty()
{
	return items_.empty();
}

size_t selectionListForm::linesNeeded(const std::string& str)
{
	return (str.size() + width_ - 1) / width_;
}

void selectionListForm::showItem(size_t x, size_t y, const std::string& str, window& wnd, bool selected)
{
	size_t edgeX_ = x_ + width_, edgeY_ = y_ + height_;
	auto tmp = x;
	for (auto ch : str)
	{
		wnd.setChar(x, y, ch);
		wnd.setAttribute(x++, y, selected && showSelected_ ? window::HIGHLIGHT_COLOR : window::DEFAULT_COLOR);
		if (x >= edgeX_)
		{
			if (horizontal_)
			{
				return;
			}
			y++;
			x = tmp;
		}
	}
	while (x < edgeX_)
	{
		wnd.setChar(x, y, ' ');
		wnd.setAttribute(x++, y, window::DEFAULT_COLOR);
	}
}
} // namespace tk