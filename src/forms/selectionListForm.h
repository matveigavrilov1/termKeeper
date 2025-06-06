#pragma once

#include <uuid.h>

#include "core/form.h"

namespace forms
{
class selectionListForm : public core::form
{
public:
	struct item {
		std::string content;
		uuids::uuid uuid;
	};
	using item_t = item;
	using item_list_t = std::vector<item_t>;
	selectionListForm(bool horizontal = false, const item_list_t& items = {});

	void show(core::window& wnd) override;

	void addItem(const item_t& item);
	void removeItem(const item_t& item);

	item_t getSelected();
	size_t selectedIndex();

	void switchUp();
	void switchDown();

	void showSelected();
	void unshowSelected();

	void clear();

	bool empty();

private:
	size_t linesNeeded(const std::string& str);
	void showItem(size_t x, size_t y, const std::string& str, core::window& wnd, bool selected = false);
	void adjustOffset();

private:
	bool showSelected_ { false };
	bool horizontal_ { false };
	size_t selectedIndex_ { 0 };
	size_t offset_ { 0 };
	item_list_t items_;
};
} // namespace forms