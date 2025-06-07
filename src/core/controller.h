#pragma once

#include <memory>

#include "core/events.h"
#include "utils/with_uuid.h"

namespace core
{
class controller : virtual public utils::with_uuid
{
public:
	virtual ~controller() = default;

	using shared_ptr_t = std::shared_ptr<controller>;

	virtual bool handleInputEvent(inputEvent::shared_ptr_t inputEvent);

protected:
	virtual bool handleKeyPressed(char ch, const inputEvent::keyModifiers& mods);
	virtual bool handleBackspace(const inputEvent::keyModifiers& mods);
	virtual bool handleEnter(const inputEvent::keyModifiers& mods);
	virtual bool handleArrowUp(const inputEvent::keyModifiers& mods);
	virtual bool handleArrowDown(const inputEvent::keyModifiers& mods);
	virtual bool handleArrowLeft(const inputEvent::keyModifiers& mods);
	virtual bool handleArrowRight(const inputEvent::keyModifiers& mods);
	virtual bool handlePageUp(const inputEvent::keyModifiers& mods);
	virtual bool handlePageDown(const inputEvent::keyModifiers& mods);
	virtual bool handleHome(const inputEvent::keyModifiers& mods);
	virtual bool handleEnd(const inputEvent::keyModifiers& mods);
	virtual bool handleInsert(const inputEvent::keyModifiers& mods);
	virtual bool handleDelete(const inputEvent::keyModifiers& mods);
	virtual bool handleF1(const inputEvent::keyModifiers& mods);
	virtual bool handleF2(const inputEvent::keyModifiers& mods);
	virtual bool handleF3(const inputEvent::keyModifiers& mods);
	virtual bool handleF4(const inputEvent::keyModifiers& mods);
	virtual bool handleF5(const inputEvent::keyModifiers& mods);
	virtual bool handleF6(const inputEvent::keyModifiers& mods);
	virtual bool handleF7(const inputEvent::keyModifiers& mods);
	virtual bool handleF8(const inputEvent::keyModifiers& mods);
	virtual bool handleF9(const inputEvent::keyModifiers& mods);
	virtual bool handleF10(const inputEvent::keyModifiers& mods);
	virtual bool handleF11(const inputEvent::keyModifiers& mods);
	virtual bool handleF12(const inputEvent::keyModifiers& mods);
	virtual bool handleUnspecified(const inputEvent::keyModifiers& mods);
};

} // namespace core