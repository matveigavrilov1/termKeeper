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
	void setChar(position_on_window pos, charInfo ch) override;

	size_t innerWidth() const;
	size_t innerHeight() const;

private:
	bool isBorder(size_t x, size_t y) const;
	void drawBorder();
};

} // namespace tk

#endif // BORDEREDWINDOW_H