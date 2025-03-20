#include "cli/forms/inputForm.h"

#include <windows.h>
#include <conio.h>
#include <algorithm>

#include "cli/core/events.h"

namespace tk
{

inputForm::inputForm(size_t x, size_t y, size_t width, size_t height, bool oneLineMode)
: form(x, y, width, height)
, oneLineMode_ { oneLineMode }
{
	lines_.push_back("");
}

void inputForm::handleInput(inputEvent::shared_ptr_type event)
{
	if (!event)
	{
		return;
	}
	switch (event->inputType())
	{
		case inputEvent::ARROW_UP:
		{
			if (!oneLineMode_)
				moveCursorUp();
		}
		break;
		case inputEvent::ARROW_DOWN:
		{
			if (!oneLineMode_)
				moveCursorDown();
		}
		break;
		case inputEvent::ARROW_LEFT:
		{
			moveCursorLeft();
		}
		break;
		case inputEvent::ARROW_RIGHT:
		{
			moveCursorRight();
		}
		break;
		case inputEvent::BACKSPACE:
		{
			backspace();
		}
		break;
		case inputEvent::KEY_PRESSED:
		{
			auto ch = *(event->key());
			if (ch)
			{
				if (cursorX_ < lines_[cursorY_].size())
				{
					lines_[cursorY_].insert(lines_[cursorY_].begin() + cursorX_, ch);
				}
				else
				{
					lines_[cursorY_] += ch;
				}
				cursorX_++;

				if (cursorX_ >= offsetX_ + width_)
				{
					offsetX_ = cursorX_ - width_ + 1;
				}
			}
		}
		break;
		case inputEvent::SHIFT_ENTER:
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
		break;
		default: break;
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

void inputForm::updateBuffer()
{
	clearBuffer();

	for (size_t row = 0; row < height_; ++row)
	{
		size_t lineIndex = row + offsetY_;
		if (lineIndex >= lines_.size())
			break;

		const std::string& line = lines_[lineIndex];
		for (size_t col = 0; col < width_; ++col)
		{
			size_t lineCol = col + offsetX_;
			if (lineCol < line.size())
			{
				buffer_[row * width_ + col].Char.AsciiChar = line[lineCol];
			}
			else
			{
				buffer_[row * width_ + col].Char.AsciiChar = ' ';
			}

			if (lineIndex == cursorY_ && lineCol == cursorX_)
			{
				buffer_[row * width_ + col].Attributes = window::HIGHLIGHT_COLOR;
			}
		}
	}
}

std::vector<std::string> inputForm::getInput() const
{
	return lines_;
}


} // namespace tk