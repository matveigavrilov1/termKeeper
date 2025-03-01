#pragma once

#include <optional>

#include "cli/core/consolem.h"
#include "cli/core/eventm.h"
#include "cli/core/screen.h"

namespace tk
{
namespace cli
{

class core
{
public:
	static void init();
	static consolem& getConsoleManager();
	static eventm& getEventManager();
	static screen& getScreen();

private:
	static void initDefaultEventHandlers();
	static bool isInitialized_;

	static std::optional<consolem> consolem_;
	static std::optional<eventm> eventm_;
	static std::optional<screen> screen_;
};

} // namespace cli
} // namespace tk