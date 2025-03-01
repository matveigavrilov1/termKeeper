#pragma once

#include "cli/core/form.h"

namespace tk {
	class selectionListForm : public form
	{
	public:
		using item_type = std::string;
		using item_list_type = std::vector<item_type>;
		selectionListForm(const item_list_type& items, size_t x, size_t y, size_t width, size_t height, bool horizontal = false);

		item_type getSelected();

		void updateBuffer();

		void switchUp();
		void switchDown();
	private:
		bool horizontal_{false};
		size_t activeIndex_{0};
		item_list_type items_;
	};
}