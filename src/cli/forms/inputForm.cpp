#include "cli/forms/inputForm.h"

#include <windows.h>
#include <conio.h>
#include <algorithm>

namespace tk
{

inputForm::inputForm(size_t x, size_t y, size_t width, size_t height, bool oneLineMode)
: form(x, y, width, height)
, oneLineMode_ { oneLineMode }
{
	lines_.push_back("");
}

void inputForm::show(window& wnd) { }

void inputForm::backspace()
{
	if (cursorX_ > 0)
	{
		lines_[cursorY_].erase(lines_[cursorY_].begin() + cursorX_ - 1);
		cursorX_--;

		if (cursorX_ < offsetX_)
		{
			offsetX_ = cursorX_;
		}
	}
	else if (cursorY_ > 0 && !oneLineMode_)
	{
		cursorX_ = lines_[cursorY_ - 1].size();
		lines_[cursorY_ - 1] += lines_[cursorY_];
		lines_.erase(lines_.begin() + cursorY_);
		cursorY_--;

		if (cursorY_ < offsetY_)
		{
			offsetY_ = cursorY_;
		}
	}
}

void inputForm::moveCursorLeft()
{
	if (cursorX_ > 0)
	{
		cursorX_--;
	}
	else if (cursorY_ > 0 && !oneLineMode_)
	{
		cursorY_--;
		cursorX_ = lines_[cursorY_].size();
	}

	if (cursorX_ < offsetX_)
	{
		offsetX_ = cursorX_;
	}
}

void inputForm::moveCursorRight()
{
	if (cursorX_ < lines_[cursorY_].size())
	{
		cursorX_++;
	}
	else if (cursorY_ < lines_.size() - 1 && !oneLineMode_)
	{
		cursorY_++;
		cursorX_ = 0;
	}

	if (cursorX_ >= offsetX_ + width_)
	{
		offsetX_ = cursorX_ - width_ + 1;
	}
}

void inputForm::moveCursorUp()
{
	if (cursorY_ > 0 && !oneLineMode_)
	{
		cursorY_--;
		cursorX_ = std::min(cursorX_, lines_[cursorY_].size());
	}

	if (cursorY_ < offsetY_)
	{
		offsetY_ = cursorY_;
	}
}

void inputForm::moveCursorDown()
{
	if (cursorY_ < lines_.size() - 1 && !oneLineMode_)
	{
		cursorY_++;
		cursorX_ = std::min(cursorX_, lines_[cursorY_].size());
	}

	if (cursorY_ >= offsetY_ + height_)
	{
		offsetY_ = cursorY_ - height_ + 1;
	}
}

void inputForm::shiftEnter()
{
	if (!oneLineMode_)
	{
		std::string leftPart = lines_[cursorY_].substr(0, cursorX_);
		std::string rightPart = lines_[cursorY_].substr(cursorX_);

		lines_[cursorY_] = leftPart;
		lines_.insert(lines_.begin() + cursorY_ + 1, rightPart);

		cursorY_++;
		cursorX_ = 0;

		if (cursorY_ >= offsetY_ + height_)
		{
			offsetY_ = cursorY_ - height_ + 1;
		}
		offsetX_ = 0;
	}
}

void inputForm::keyPressed(char key)
{
	if (key)
	{
		if (cursorX_ < lines_[cursorY_].size())
		{
			lines_[cursorY_].insert(lines_[cursorY_].begin() + cursorX_, key);
		}
		else
		{
			lines_[cursorY_] += key;
		}
		cursorX_++;

		if (cursorX_ >= offsetX_ + width_)
		{
			offsetX_ = cursorX_ - width_ + 1;
		}
	}
}

void inputForm::setInput(std::vector<std::string> input)
{
	clear();
	lines_ = std::move(input);
}

std::vector<std::string> inputForm::getInput() const
{
	return lines_;
}

void inputForm::clear()
{
	cursorX_ = 0;
	cursorY_ = 0;
	offsetX_ = 0;
	offsetY_ = 0;
	lines_ = { "" };
}

} // namespace tk