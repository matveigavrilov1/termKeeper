#pragma once

#include <optional>

#include "core/controllerm.h"
#include "core/eventm.h"
#include "core/screen.h"

namespace core
{
void init();

controllerm& controllerm();
eventm& eventm();
screen& screen();
} // namespace core