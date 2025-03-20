#pragma once

#include "cli/core/form.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace tk
{

class hintsForm : public form
{
public:
    using hint_type = std::pair<std::string, std::string>;
	using preset_type = std::vector<hint_type>;
    using preset_name_type = std::string;

	hintsForm(size_t x, size_t y, size_t width, size_t height);

	void addHint(const std::string& key, const std::string& description);


	void clearHints();

	void addPreset(const std::string& presetName, const std::vector<std::pair<std::string, std::string>>& hints);

	void applyPreset(const std::string& presetName);

	void updateBuffer();

private:
	std::vector<hint_type> hints_;

	std::unordered_map<preset_name_type, preset_type> presets_;

	void addTextToBuffer(const std::string& text, size_t& row, size_t& col, WORD attributes);
};
} // namespace tk