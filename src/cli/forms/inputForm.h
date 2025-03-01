#include "cli/core/form.h"

#include "cli/core/events.h"

namespace tk
{
class inputForm : public form
{
public:
	inputForm(size_t x, size_t y, size_t width, size_t height);
	virtual ~inputForm() = default;

	void handleInput(inputEvent::shared_ptr_type event);
	void backspace();
	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorUp();
	void moveCursorDown();
	void updateBuffer();

	std::vector<std::string> getInput() const;

private:
	std::vector<std::string> lines_; // Многострочный текст
	size_t cursorX_ { 0 };					 // Позиция курсора по X (внутри строки)
	size_t cursorY_ { 0 };					 // Позиция курсора по Y (номер строки)
};
} // namespace tk
