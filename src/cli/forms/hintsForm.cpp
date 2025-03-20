#include "cli/forms/hintsForm.h"

namespace tk
{

hintsForm::hintsForm(size_t x, size_t y, size_t width, size_t height)
: form(x, y, width, height)
{ }

void hintsForm::addHint(const std::string& key, const std::string& description)
{
	hints_.emplace_back(key, description);
	updateBuffer();
}

void hintsForm::clearHints()
{
	hints_.clear();
	updateBuffer();
}

void hintsForm::addPreset(const std::string& presetName, const std::vector<std::pair<std::string, std::string>>& hints)
{
	presets_[presetName] = hints;
}

void hintsForm::applyPreset(const std::string& presetName)
{
	auto it = presets_.find(presetName);
	if (it != presets_.end())
	{
		hints_ = it->second;
		updateBuffer();
	}
}

void hintsForm::updateBuffer()
{
	clearBuffer(); 

	size_t row = 0;
	size_t col = 0;

	for (const auto& hint : hints_)
	{
		addTextToBuffer(hint.first, row, col, window::HIGHLIGHT_COLOR);

		addTextToBuffer(":" + hint.second + " ", row, col, window::DEFAULT_COLOR);

		if (col >= width_)
		{
			row++;
			col = 0;
			if (row >= height_)
			{
				break; 
			}
		}
	}
}

void hintsForm::addTextToBuffer(const std::string& text, size_t& row, size_t& col, WORD attributes)
{
	for (char ch : text)
	{
		if (col >= width_)
		{
			row++;
			col = 0;
			if (row >= height_)
			{
				break; // Если достигли предела ширины формы, переходим на следующую строку
			}
		}

		buffer_[row * width_ + col].Char.AsciiChar = ch;
		buffer_[row * width_ + col].Attributes = attributes;
		col++;
	}
}
} // namespace tk