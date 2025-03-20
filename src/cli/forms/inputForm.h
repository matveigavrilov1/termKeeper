#include "cli/core/form.h"

namespace tk
{
class inputForm : public form
{
public:
	inputForm(size_t x, size_t y, size_t width, size_t height, bool oneLineMode = false);
	virtual ~inputForm() = default;

	void backspace();
	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorUp();
	void moveCursorDown();
	void shiftEnter();
	void keyPressed(char key);
	void updateBuffer();

	void clear();

	void setInput(std::vector<std::string> input);
	std::vector<std::string> getInput() const;

private:
	std::vector<std::string> lines_;
	size_t cursorX_ { 0 };
	size_t cursorY_ { 0 };
	size_t offsetX_ { 0 };
	size_t offsetY_ { 0 };
	bool oneLineMode_ { false };
};
} // namespace tk
