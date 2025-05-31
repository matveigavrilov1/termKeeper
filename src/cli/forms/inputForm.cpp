#include "cli/forms/inputForm.h"

#include <conio.h>
#include <algorithm>

namespace tk
{

void inputForm::show(window& wnd)
{
	form::show(wnd);

	if (x() >= wnd.width() || y() >= wnd.height())
		return;

	size_t edgeX = std::min(x() + width(), wnd.width());
	size_t edgeY = std::min(y() + height(), wnd.height());

	for (size_t yIt = y(); yIt < edgeY; ++yIt)
	{
		size_t lineIndex = yIt + offsetY_;

		const std::string& line = lineIndex < lines_.size() ? lines_[lineIndex] : "";
		for (size_t xIt = x(); xIt < edgeX; ++xIt)
		{
			size_t lineCol = xIt + offsetX_;
			auto bgColor = (lineIndex == cursorY_ && lineCol == cursorX_) ? os::console::color::CONSOLE_COLOR_WHITE : os::console::color::CONSOLE_COLOR_BLACK;
			auto txtColor = (lineIndex == cursorY_ && lineCol == cursorX_) ? os::console::color::CONSOLE_COLOR_BLACK : os::console::color::CONSOLE_COLOR_WHITE;
			unsigned char ch = (lineCol < line.size()) ? line[lineCol] : ' ';
			wnd.setChar({ xIt, yIt }, { ch, bgColor, txtColor });
		}
	}
}

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

	if (cursorX_ >= offsetX_ + width())
	{
		offsetX_ = cursorX_ - width() + 1;
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

	if (cursorY_ >= offsetY_ + height())
	{
		offsetY_ = cursorY_ - height() + 1;
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

		if (cursorY_ >= offsetY_ + height())
		{
			offsetY_ = cursorY_ - height() + 1;
		}
		offsetX_ = 0;
	}
}

void inputForm::home()
{
	cursorX_ = 0;
	offsetX_ = 0;
}

void inputForm::end()
{
	cursorX_ = lines_[cursorY_].size();
	if (cursorX_ >= offsetX_ + width())
	{
		offsetX_ = cursorX_ - width() + 1;
	}
}

void inputForm::deleteChar()
{
	if (cursorX_ < lines_[cursorY_].size())
	{
		lines_[cursorY_].erase(lines_[cursorY_].begin() + cursorX_);
	}
	else if (cursorY_ < lines_.size() - 1 && !oneLineMode_)
	{
		// Merge with next line if at end of current line
		lines_[cursorY_] += lines_[cursorY_ + 1];
		lines_.erase(lines_.begin() + cursorY_ + 1);
	}
}

void inputForm::insertChar(char key)
{
	if (key)
	{
		if (insertMode_ && cursorX_ < lines_[cursorY_].size())
		{
			lines_[cursorY_][cursorX_] = key;
		}
		else
		{
			if (cursorX_ < lines_[cursorY_].size())
			{
				lines_[cursorY_].insert(lines_[cursorY_].begin() + cursorX_, key);
			}
			else
			{
				lines_[cursorY_] += key;
			}
		}
		cursorX_++;

		if (cursorX_ >= offsetX_ + width())
		{
			offsetX_ = cursorX_ - width() + 1;
		}
	}
}

void inputForm::keyPressed(char key)
{
	insertChar(key);
}

void inputForm::toggleInsertMode()
{
	insertMode_ = !insertMode_;
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