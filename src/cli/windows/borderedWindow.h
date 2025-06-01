#ifndef BORDEREDWINDOW_H
#define BORDEREDWINDOW_H

#include "cli/core/window.h"

namespace tk
{

class borderedWindow : public window
{
public:
	using window::window;

	void clear() override;

	position_on_screen contentPos() const override;
	window_size contentSize() const override;

private:
	bool isBorder(size_t x, size_t y) const;
	void drawBorder();
};

} // namespace tk

#endif // BORDEREDWINDOW_H