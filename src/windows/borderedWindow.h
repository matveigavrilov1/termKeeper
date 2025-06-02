#ifndef BORDEREDWINDOW_H
#define BORDEREDWINDOW_H

#include "core/window.h"

namespace wndws
{

class borderedWindow : public core::window
{
public:
	using core::window::window;

	void clear() override;

	position_on_screen contentPos() const override;
	window_size contentSize() const override;

	void setHighlightTitle(bool highlight);

	bool isBorder(size_t x, size_t y) const;
	void drawBorder();
	void drawTitle();

private:
	bool highlightTitle_ = false;
};

} // namespace wndws

#endif // BORDEREDWINDOW_H