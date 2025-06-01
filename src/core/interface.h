#pragma once

#include <optional>

#include "core/eventm.h"

#include "core/screen.h"

namespace tk
{
namespace ui
{

class core
{
public:
	static void init();

	static eventm& getEventManager();
	static screen& getScreen();

private:
	static bool isInitialized_;

	static std::optional<eventm> eventm_;
	static std::optional<screen> screen_;
};

} // namespace ui
} // namespace tk