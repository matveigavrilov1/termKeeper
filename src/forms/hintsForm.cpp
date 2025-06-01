#include "forms/hintsForm.h"

#include "os/console.h"

namespace tk
{

void hintsForm::show(window& wnd)
{
	form::show(wnd);

	if (!hints_)
	{
		return;
	}

	size_t startX = x();
	size_t startY = y();
	const size_t edgeX = x() + width();
	const size_t edgeY = x() + height();

	auto setContentCharWithWrap = [&](unsigned char ch, os::console::color color) -> bool
	{
		wnd.setContentChar({ startX, startY }, { .ch = ch, .bgColor = color, .txtColor = os::console::CONSOLE_COLOR_BLACK });

		if (++startX >= edgeX)
		{
			startX = x();
			if (++startY >= edgeY)
			{
				return false;
			}
		}
		return true;
	};

	auto writeString = [&](const std::string& str, os::console::color color) -> bool
	{
		for (char ch : str)
		{
			if (!setContentCharWithWrap(ch, color))
			{
				return false;
			}
		}
		return true;
	};


	for (const auto& hint : *hints_)
	{
		if (!writeString(hint.first, os::console::CONSOLE_COLOR_WHITE))
			return;


		if (!setContentCharWithWrap(':', os::console::CONSOLE_COLOR_BLACK))
			return;


		if (!writeString(hint.second, os::console::CONSOLE_COLOR_BLACK))
			return;


		if (!setContentCharWithWrap(' ', os::console::CONSOLE_COLOR_BLACK))
			return;
	}

	while (startY < edgeY)
	{
		if (!setContentCharWithWrap(' ', os::console::CONSOLE_COLOR_BLACK))
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