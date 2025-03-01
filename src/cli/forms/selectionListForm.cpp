#include "cli/forms/selectionListForm.h"

namespace tk
{

selectionListForm::selectionListForm(const item_list_type& items, size_t x, size_t y, size_t width, size_t height, bool horizontal)
: form(x, y, width, height)
, items_(items)
, horizontal_(horizontal)
{ }

selectionListForm::item_type selectionListForm::getSelected()
{
	return items_[activeIndex_];
}

static bool insertStringsWithSpace(const std::vector<std::string>& items, std::vector<CHAR_INFO>& buffer, size_t width, size_t height, size_t activeIndex)
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
			if (activeIndex == index)
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
			if (activeIndex == index)
			{
				buffer[row * width + col].Attributes = window::HIGHLIGHT_COLOR;
			}
			col++;
		}

		index++;
	}

	return true;
}

static bool insertStringsLineByLine(const std::vector<std::string>& items, std::vector<CHAR_INFO>& buffer, size_t width, size_t height, size_t activeIndex)
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
			if (activeIndex == index)
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
	clear();
	if (horizontal_)
	{
		insertStringsWithSpace(items_, buffer_, width_, height_, activeIndex_);
	}
	else
	{
		insertStringsLineByLine(items_, buffer_, width_, height_, activeIndex_);
	}
}

void selectionListForm::switchUp()
{
	if (activeIndex_ == 0)
	{
		activeIndex_ = items_.size() - 1;
	}
	else
	{
		activeIndex_ -= 1;
	}
}

void selectionListForm::switchDown()
{
	if (activeIndex_ == items_.size() - 1)
	{
		activeIndex_ = 0;
	}
	else
	{
		activeIndex_ += 1;
	}
}
} // namespace tk