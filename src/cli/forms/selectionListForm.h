#pragma once

#include "cli/core/form.h"

namespace tk
{
class selectionListForm : public form
{
public:
	using item_type = std::string;
	using item_list_type = std::vector<item_type>;
	selectionListForm(size_t x, size_t y, size_t width, size_t height, bool horizontal = false, const item_list_type& items = {});

	void addItem(const item_type& item);
	void removeItem(const item_type& item);

	item_type getSelected();
	size_t selectedIndex();

	void updateBuffer();

	void switchUp();
	void switchDown();

	void showSelected();
	void unshowSelected();

	void clear();
	
	bool empty();
private:
	bool showSelected_ { false };
	bool horizontal_ { false };
	size_t selectedIndex_ { 0 };
	item_list_type items_;
};
} // namespace tk