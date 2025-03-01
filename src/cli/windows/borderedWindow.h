#ifndef BORDEREDWINDOW_H
#define BORDEREDWINDOW_H

#include "cli/core/window.h"
#include <string>

namespace tk
{

class borderedWindow : public window
{
public:
	borderedWindow(size_t x, size_t y, size_t width, size_t height, const std::string& title);
	borderedWindow(const borderedWindow& other);
	borderedWindow(borderedWindow&& other) noexcept;

	borderedWindow& operator= (const borderedWindow& other);
	borderedWindow& operator= (borderedWindow&& other) noexcept;

	void setTitle(const std::string& title);

	void clear() override;
	void setChar(size_t x, size_t y, char ch) override;
	void setAttribute(size_t x, size_t y, WORD attr) override;

	size_t innerWidth() const;
	size_t innerHeight() const;

private:
	bool isBorder(size_t x, size_t y) const;
	void drawBorder();

private:
	std::string title_;
};

} // namespace tk

#endif // BORDEREDWINDOW_H