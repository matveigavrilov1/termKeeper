#pragma once

#include <string>

#include <data/actions.h>

namespace os 
{
	int runApp(const char * appName);
	void writeToClipboard(const std::string& content);
	void execute(data::action::shared_ptr_t action);
}