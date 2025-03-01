#include "cli/forms/inputForm.h"

#include <windows.h>
#include <conio.h>
#include <algorithm>

namespace tk
{

inputForm::inputForm(size_t x, size_t y, size_t width, size_t height)
: form(x, y, width, height)
{
	lines_.push_back("");
}

void inputForm::handleInput(inputEvent::shared_ptr_type event)
{
	updateBuffer();
}

void inputForm::backspace()
{
	if (cursorX_ > 0)
	{
		lines_[cursorY_].erase(lines_[cursorY_].begin() + cursorX_ - 1);
		cursorX_--;
	}
	else if (cursorY_ > 0)
	{
		cursorX_ = lines_[cursorY_ - 1].size();
		lines_[cursorY_ - 1] += lines_[cursorY_];
		lines_.erase(lines_.begin() + cursorY_);
		cursorY_--;
	}
	updateBuffer();
}

void inputForm::moveCursorLeft()
{
	if (cursorX_ > 0)
	{
		cursorX_--;
	}
	else if (cursorY_ > 0)
	{
		cursorY_--;
		cursorX_ = lines_[cursorY_].size();
	}
	updateBuffer();
}

void inputForm::moveCursorRight()
{
	if (cursorX_ < lines_[cursorY_].size())
	{
		cursorX_++;
	}
	else if (cursorY_ < lines_.size() - 1)
	{
		cursorY_++;
		cursorX_ = 0;
	}
	updateBuffer();
}

void inputForm::moveCursorUp()
{
	if (cursorY_ > 0)
	{
		cursorY_--;
		cursorX_ = std::min(cursorX_, lines_[cursorY_].size());
	}
	updateBuffer();
}

void inputForm::moveCursorDown()
{
	if (cursorY_ < lines_.size() - 1)
	{
		cursorY_++;
		cursorX_ = std::min(cursorX_, lines_[cursorY_].size());
	}
	updateBuffer();
}

void inputForm::updateBuffer()
{
	clear();

	for (size_t row = 0; row < lines_.size() && row < height_; ++row)
	{
		const std::string& line = lines_[row];
		for (size_t col = 0; col < line.size() && col < width_; ++col)
		{
			buffer_[row * width_ + col].Char.AsciiChar = line[col];
			if (row == cursorY_ && col == cursorX_)
			{
				buffer_[row * width_ + col].Attributes = window::HIGHLIGHT_COLOR;
			}
		}

		if (row == cursorY_ && cursorX_ == line.size() && cursorX_ < width_)
		{
			buffer_[row * width_ + cursorX_].Char.AsciiChar = ' ';
			buffer_[row * width_ + cursorX_].Attributes = window::HIGHLIGHT_COLOR;
		}
	}
}

std::vector<std::string> inputForm::getInput() const
{
	return lines_;
}

} // namespace tk