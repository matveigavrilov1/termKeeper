#include "cli/forms/selectionListForm.h"

namespace tk
{

selectionListForm::selectionListForm(size_t x, size_t y, size_t width, size_t height, bool horizontal, const item_list_type& items)
: form(x, y, width, height)
, items_(items)
, horizontal_(horizontal)
{ }

void selectionListForm::addItem(const item_type& item)
{
	items_.push_back(item);
}

void selectionListForm::removeItem(const item_type& item) { }

selectionListForm::item_type selectionListForm::getSelected()
{
	if (items_.empty())
	{
		return {};
	}
	return items_[selectedIndex_];
}

size_t selectionListForm::selectedIndex()
{
	return selectedIndex_;
}

static bool insertStringsWithSpace(
	const std::vector<std::string>& items, std::vector<CHAR_INFO>& buffer, size_t width, size_t height, size_t activeIndex, bool showSelected)
{
	size_t row = 0;
	size_t col = 0;
	size_t index = 0;
	for (const auto& item : items)
	{
		if (row >= height)
		{
			return false;
		}

		for (char ch : item)
		{
			if (col >= width)
			{
				row++;
				col = 0;
				if (row >= height)
				{
					return false;
				}
			}

			buffer[row * width + col].Char.AsciiChar = ch;
			if (activeIndex == index && showSelected)
			{
				buffer[row * width + col].Attributes = window::HIGHLIGHT_COLOR;
			}
			col++;
		}

		if (col >= width)
		{
			row++;
			col = 0;
			if (row >= height)
			{
				return false;
			}
		}

		if (col < width)
		{
			buffer[row * width + col].Char.AsciiChar = ' ';
			buffer[row * width + col].Attributes = window::DEFAULT_COLOR;

			col++;
		}

		index++;
	}

	return true;
}

static bool insertStringsLineByLine(
	const std::vector<std::string>& items, std::vector<CHAR_INFO>& buffer, size_t width, size_t height, size_t activeIndex, bool showSelected)
{
	size_t row = 0;
	size_t col = 0;
	size_t index = 0;
	for (const auto& item : items)
	{
		if (row >= height)
		{
			return false;
		}

		for (char ch : item)
		{
			if (col >= width)
			{
				row++;
				col = 0;
				if (row >= height)
				{
					return false;
				}
			}

			buffer[row * width + col].Char.AsciiChar = ch;
			if (activeIndex == index && showSelected)
			{
				buffer[row * width + col].Attributes = window::HIGHLIGHT_COLOR;
			}
			col++;
		}

		row++;
		index++;
		col = 0;
	}

	return true;
}

void selectionListForm::updateBuffer()
{
	clearBuffer();
	if (horizontal_)
	{
		insertStringsWithSpace(items_, buffer_, width_, height_, selectedIndex_, showSelected_);
	}
	else
	{
		insertStringsLineByLine(items_, buffer_, width_, height_, selectedIndex_, showSelected_);
	}
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
	updateBuffer();
}

bool selectionListForm::empty()
{
	return items_.empty();
}

} // namespace tk