#pragma once

#include <functional>

#include "utils/singleton.h"

namespace utils
{
class visibilityManager : public singleton<visibilityManager>
{
public:
    using on_hide_callback_t = std::function<void()>;

	void hide();
    void addCallback(on_hide_callback_t callback);

private:
    std::vector<on_hide_callback_t> callbacks_;
};
} // namespace utils