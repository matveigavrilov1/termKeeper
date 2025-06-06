#pragma once

#include <string>

namespace os 
{
	int runApp(const char * appName);
	void writeToClipboard(const std::string& content);
}