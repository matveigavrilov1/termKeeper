#include "cli/forms/hintsForm.h"

namespace tk
{

hintsForm::hintsForm(size_t x, size_t y, size_t width, size_t height)
: form(x, y, width, height)
{ }

void hintsForm::show(window& wnd) { }

void hintsForm::addHint(const std::string& key, const std::string& description)
{
	hints_.emplace_back(key, description);
}

void hintsForm::clearHints()
{
	hints_.clear();
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
	}
}

} // namespace tk