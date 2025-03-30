#include "cli/forms/hintsForm.h"

namespace tk
{

hintsForm::hintsForm(size_t x, size_t y, size_t width, size_t height)
: form(x, y, width, height)
{ }

void hintsForm::show(window& wnd)
{
	if (!hints_)
	{
		return;
	}

	size_t x = x_;
	size_t y = y_;
	const size_t edgeX = x_ + width_;
	const size_t edgeY = y_ + height_;

	auto setCharWithWrap = [&](char ch, int color) -> bool
	{
		wnd.setChar(x, y, ch);
		wnd.setAttribute(x, y, color);

		if (++x >= edgeX)
		{
			x = x_;
			if (++y >= edgeY)
			{
				return false;
			}
		}
		return true;
	};

	auto writeString = [&](const std::string& str, int color) -> bool
	{
		for (char ch : str)
		{
			if (!setCharWithWrap(ch, color))
			{
				return false;
			}
		}
		return true;
	};


	for (const auto& hint : *hints_)
	{
		if (!writeString(hint.first, window::HIGHLIGHT_COLOR))
			return;


		if (!setCharWithWrap(':', window::DEFAULT_COLOR))
			return;


		if (!writeString(hint.second, window::DEFAULT_COLOR))
			return;


		if (!setCharWithWrap(' ', window::DEFAULT_COLOR))
			return;
	}

	while (y < edgeY)
	{
		if (!setCharWithWrap(' ', window::DEFAULT_COLOR))
			return;
	}
}

void hintsForm::addHint(const std::string& key, const std::string& description)
{
	if (hints_)
	{
		hints_->emplace_back(key, description);
	}
}

void hintsForm::clearHints()
{
	if (hints_)
	{
		hints_->clear();
	}
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
		hints_ = &it->second;
	}
}

} // namespace tk