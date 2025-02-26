#pragma once

#include "os/clipboardController/interface.h"

namespace tk
{
class windowsClipboardController : public clipboardController
{
public:
	void write(const std::string & content) override;
};
}; // namespace tk