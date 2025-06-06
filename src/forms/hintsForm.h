#pragma once

#include "core/form.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace forms
{

class hintsForm : public core::form
{
public:
	using hint_t = std::pair<std::string, std::string>;
	using preset_t = std::vector<hint_t>;
	using preset_name_t = std::string;

	using form::form;

	void show(core::window& wnd) override;

	void addHint(const std::string& key, const std::string& description);

	void clearHints();

	void addPreset(const std::string& presetName, const std::vector<std::pair<std::string, std::string>>& hints);

	void applyPreset(const std::string& presetName);

private:
	preset_t* hints_ { nullptr };

	std::unordered_map<preset_name_t, preset_t> presets_;

	void addTextToBuffer(const std::string& text, size_t& row, size_t& col);
};
} // namespace forms