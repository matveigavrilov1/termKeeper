#include "cli/forms/selectionListForm.h"

namespace tk
{

selectionListForm::selectionListForm(bool horizontal, const item_list_type& items)
: items_(items)
, horizontal_(horizontal)
, offset_(0)
{ }

void selectionListForm::show(window& wnd)
{
	form::show(wnd);


	size_t startX = x(), startY = y();
	size_t edgeX = x() + width(), edgeY = y() + height();
	size_t index = offset_;


	while (startY < edgeY && startX < edgeX && index < items_.size())
	{
		const auto& item = items_[index];

		showItem(startX, startY, items_[index].content, wnd, index == selectedIndex_);

		if (horizontal_)
		{
			startX += item.content.size() + 1;
			if (startX >= edgeX)
			{
				break;
			}
		}
		else
		{
			auto lines = linesNeeded(item.content);

			auto nextY = startY + lines;
			if (nextY > edgeY)
			{
				break;
			}
			startY = nextY;
		}
		++index;
	}

	if (horizontal_)
	{
		startY++;
	}


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

void selectionListForm::addItem(const item_type& item)
{
	items_.push_back(item);


	adjustOffset();
}

void selectionListForm::removeItem(const item_type& item)
{
	// Реализация удаления должна быть добавлена
}

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
	adjustOffset();
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
	adjustOffset();
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
	offset_ = 0;
}

bool selectionListForm::empty()
{
	return items_.empty();
}

size_t selectionListForm::linesNeeded(const std::string& str)
{
	if (!width())
	{
		return 1;
	}
	size_t lines = (str.size() + width() - 1) / width();

	return lines;
}

void selectionListForm::showItem(size_t startX, size_t startY, const std::string& str, window& wnd, bool selected)
{
	size_t edgeX = pos_.x + width(), edgeY = pos_.y + height();
	auto tmp = startX;


	for (auto ch : str)
	{
		if (startX >= edgeX || startY >= edgeY)
		{
			break;
		}

		auto bgColor = (selected && showSelected_) ? os::console::color::CONSOLE_COLOR_WHITE : os::console::color::CONSOLE_COLOR_BLACK;
		auto txtColor = (selected && showSelected_) ? os::console::color::CONSOLE_COLOR_BLACK : os::console::color::CONSOLE_COLOR_WHITE;


		wnd.setContentChar({ startX++, startY }, { static_cast<unsigned char>(ch), bgColor, txtColor });

		if (startX >= edgeX)
		{
			if (horizontal_)
			{
				return;
			}
			startY++;
			startX = tmp;
		}
	}


	while (startX < edgeX && startY < edgeY)
	{
		wnd.setContentChar({ startX++, startY }, { ' ' });
	}
}

void selectionListForm::adjustOffset()
{
	if (items_.empty())
	{
		return;
	}

	if (horizontal_)
	{
		offset_ = 0;
		return;
	}

	size_t visibleItems = 0;
	size_t y = pos_.y;
	size_t edgeY_ = pos_.y + height();


	for (size_t i = offset_; i < items_.size(); ++i)
	{
		size_t needed = linesNeeded(items_[i].content);

		if (y + needed > edgeY_)
		{
			break;
		}
		y += needed;
		visibleItems++;
	}


	if (selectedIndex_ < offset_)
	{
		offset_ = selectedIndex_;
	}
	else if (selectedIndex_ >= offset_ + visibleItems)
	{
		size_t newOffset = selectedIndex_;
		y = pos_.y;

		while (newOffset > 0)
		{
			size_t needed = linesNeeded(items_[newOffset].content);

			if (y + needed > edgeY_)
			{
				break;
			}
			y += needed;
			newOffset--;
		}

		offset_ = newOffset;
		if (y + linesNeeded(items_[selectedIndex_].content) > edgeY_)
		{
			offset_++;
		}
	}
}

} // namespace tk