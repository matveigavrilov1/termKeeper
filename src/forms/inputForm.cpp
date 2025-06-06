#include "forms/inputForm.h"

#include <conio.h>
#include <algorithm>

#include "utils/logger.h"

namespace forms
{

void inputForm::show(core::window& wnd)
{
	LOG_DBG("Entering inputForm::show()");
	LOG_DBG("Window content dimensions - width: " << wnd.contentWidth() << ", height: " << wnd.contentHeight());
	LOG_DBG("Form position - x: " << x() << ", y: " << y() << ", width: " << width() << ", height: " << height());

	form::show(wnd);

	if (x() >= wnd.contentWidth() || y() >= wnd.contentHeight())
	{
		LOG_DBG("Form is outside window boundaries, skipping rendering");
		return;
	}

	size_t edgeX = std::min(x() + width(), wnd.contentWidth());
	size_t edgeY = std::min(y() + height(), wnd.contentHeight());
	LOG_DBG("Rendering edges - edgeX: " << edgeX << ", edgeY: " << edgeY);

	for (size_t yIt = y(); yIt < edgeY; ++yIt)
	{
		size_t lineIndex = yIt + offsetY_;
		LOG_DBG("Processing line - yIt: " << yIt << ", lineIndex: " << lineIndex);

		const std::string& line = lineIndex < lines_.size() ? lines_[lineIndex] : "";
		LOG_DBG("Line content: '" << line << "'");

		for (size_t xIt = x(); xIt < edgeX; ++xIt)
		{
			size_t lineCol = xIt + offsetX_;
			auto bgColor = (lineIndex == cursorY_ && lineCol == cursorX_) ? os::console::color::CONSOLE_COLOR_WHITE : os::console::color::CONSOLE_COLOR_BLACK;
			auto txtColor = (lineIndex == cursorY_ && lineCol == cursorX_) ? os::console::color::CONSOLE_COLOR_BLACK : os::console::color::CONSOLE_COLOR_WHITE;
			unsigned char ch = (lineCol < line.size()) ? line[lineCol] : ' ';

			LOG_DBG("Setting char at position - x: " << xIt << ", y: " << yIt << ", char: '" << ch << "', bgColor: " << static_cast<int>(bgColor)
																							 << ", txtColor: " << static_cast<int>(txtColor));

			wnd.setContentChar({ xIt, yIt }, { ch, bgColor, txtColor });
		}
	}
	LOG_DBG("Finished inputForm::show()");
}

void inputForm::backspace()
{
	LOG_DBG("Entering backspace() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

	if (cursorX_ > 0)
	{
		LOG_DBG("Deleting character at position " << (cursorX_ - 1));
		lines_[cursorY_].erase(lines_[cursorY_].begin() + cursorX_ - 1);
		cursorX_--;

		if (cursorX_ < offsetX_)
		{
			LOG_DBG("Adjusting offsetX_ to: " << cursorX_);
			offsetX_ = cursorX_;
		}
	}
	else if (cursorY_ > 0 && !oneLineMode_)
	{
		LOG_DBG("Merging with previous line");
		cursorX_ = lines_[cursorY_ - 1].size();
		lines_[cursorY_ - 1] += lines_[cursorY_];
		lines_.erase(lines_.begin() + cursorY_);
		cursorY_--;

		if (cursorY_ < offsetY_)
		{
			LOG_DBG("Adjusting offsetY_ to: " << cursorY_);
			offsetY_ = cursorY_;
		}
	}
	LOG_DBG("After backspace - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::moveCursorLeft()
{
	LOG_DBG("Entering moveCursorLeft() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

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
		LOG_DBG("Adjusting offsetX_ to: " << cursorX_);
		offsetX_ = cursorX_;
	}

	LOG_DBG("After moveCursorLeft - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::moveCursorRight()
{
	LOG_DBG("Entering moveCursorRight() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

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
		LOG_DBG("Adjusting offsetX_ to: " << (cursorX_ - width() + 1));
		offsetX_ = cursorX_ - width() + 1;
	}

	LOG_DBG("After moveCursorRight - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::moveCursorUp()
{
	LOG_DBG("Entering moveCursorUp() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

	if (cursorY_ > 0 && !oneLineMode_)
	{
		cursorY_--;
		cursorX_ = std::min(cursorX_, lines_[cursorY_].size());
	}

	if (cursorY_ < offsetY_)
	{
		LOG_DBG("Adjusting offsetY_ to: " << cursorY_);
		offsetY_ = cursorY_;
	}

	LOG_DBG("After moveCursorUp - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::moveCursorDown()
{
	LOG_DBG("Entering moveCursorDown() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

	if (cursorY_ < lines_.size() - 1 && !oneLineMode_)
	{
		cursorY_++;
		cursorX_ = std::min(cursorX_, lines_[cursorY_].size());
	}

	if (cursorY_ >= offsetY_ + height())
	{
		LOG_DBG("Adjusting offsetY_ to: " << (cursorY_ - height() + 1));
		offsetY_ = cursorY_ - height() + 1;
	}

	LOG_DBG("After moveCursorDown - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::shiftEnter()
{
	LOG_DBG("Entering shiftEnter() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

	if (!oneLineMode_)
	{
		std::string leftPart = lines_[cursorY_].substr(0, cursorX_);
		std::string rightPart = lines_[cursorY_].substr(cursorX_);

		LOG_DBG("Splitting line - left: '" << leftPart << "', right: '" << rightPart << "'");

		lines_[cursorY_] = leftPart;
		lines_.insert(lines_.begin() + cursorY_ + 1, rightPart);

		cursorY_++;
		cursorX_ = 0;

		if (cursorY_ >= offsetY_ + height())
		{
			LOG_DBG("Adjusting offsetY_ to: " << (cursorY_ - height() + 1));
			offsetY_ = cursorY_ - height() + 1;
		}
		offsetX_ = 0;
	}

	LOG_DBG("After shiftEnter - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::home()
{
	LOG_DBG("Entering home() - cursorX_: " << cursorX_);
	cursorX_ = 0;
	offsetX_ = 0;
	LOG_DBG("After home() - cursorX_: " << cursorX_);
}

void inputForm::end()
{
	LOG_DBG("Entering end() - cursorX_: " << cursorX_ << ", line length: " << lines_[cursorY_].size());
	cursorX_ = lines_[cursorY_].size();
	if (cursorX_ >= offsetX_ + width())
	{
		LOG_DBG("Adjusting offsetX_ to: " << (cursorX_ - width() + 1));
		offsetX_ = cursorX_ - width() + 1;
	}
	LOG_DBG("After end() - cursorX_: " << cursorX_);
}

void inputForm::deleteChar()
{
	LOG_DBG("Entering deleteChar() - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);

	if (cursorX_ < lines_[cursorY_].size())
	{
		LOG_DBG("Deleting character at position " << cursorX_);
		lines_[cursorY_].erase(lines_[cursorY_].begin() + cursorX_);
	}
	else if (cursorY_ < lines_.size() - 1 && !oneLineMode_)
	{
		LOG_DBG("Merging with next line");
		lines_[cursorY_] += lines_[cursorY_ + 1];
		lines_.erase(lines_.begin() + cursorY_ + 1);
	}

	LOG_DBG("After deleteChar - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::insertChar(char key)
{
	LOG_DBG("Entering insertChar() - key: '" << key << "', cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_ << ", insertMode_: " << insertMode_);

	if (key)
	{
		if (insertMode_ && cursorX_ < lines_[cursorY_].size())
		{
			LOG_DBG("Overwriting character at position " << cursorX_);
			lines_[cursorY_][cursorX_] = key;
		}
		else
		{
			if (cursorX_ < lines_[cursorY_].size())
			{
				LOG_DBG("Inserting character at position " << cursorX_);
				lines_[cursorY_].insert(lines_[cursorY_].begin() + cursorX_, key);
			}
			else
			{
				LOG_DBG("Appending character at end of line");
				lines_[cursorY_] += key;
			}
		}
		cursorX_++;

		if (cursorX_ >= offsetX_ + width())
		{
			LOG_DBG("Adjusting offsetX_ to: " << (cursorX_ - width() + 1));
			offsetX_ = cursorX_ - width() + 1;
		}
	}

	LOG_DBG("After insertChar - cursorX_: " << cursorX_ << ", cursorY_: " << cursorY_);
}

void inputForm::keyPressed(char key)
{
	LOG_DBG("Entering keyPressed() - key: '" << key << "'");
	insertChar(key);
	LOG_DBG("Exiting keyPressed()");
}

void inputForm::toggleInsertMode()
{
	LOG_DBG("Entering toggleInsertMode() - current insertMode_: " << insertMode_);
	insertMode_ = !insertMode_;
	LOG_DBG("After toggle - insertMode_: " << insertMode_);
}

void inputForm::setInput(std::vector<std::string> input)
{
	LOG_DBG("Entering setInput() - input size: " << input.size());
	clear();
	lines_ = std::move(input);
	LOG_DBG("After setInput - lines_ size: " << lines_.size());
}

std::vector<std::string> inputForm::getInput() const
{
	LOG_DBG("Entering getInput() - lines_ size: " << lines_.size());
	return lines_;
}

void inputForm::clear()
{
	LOG_DBG("Entering clear()");
	cursorX_ = 0;
	cursorY_ = 0;
	offsetX_ = 0;
	offsetY_ = 0;
	lines_ = { "" };
	LOG_DBG("After clear - lines_ size: " << lines_.size());
}

} // namespace forms