#include "logger.h"

std::string getFileName(const std::string& filePath)
{
	size_t pos = filePath.find_last_of("/\\");
	if (pos == std::string::npos)
	{
		return filePath;
	}
	return filePath.substr(pos + 1);
}