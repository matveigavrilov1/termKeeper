#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "cli/cliCore.h"

// TODO: Optimize logs
// clang-format off
#define LOG_INF(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[INF] | "<< message;                   \
		tk::cliCore::log(ss.str());                   \
	} while (0)

#define LOG_DBG(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[DBG] | "<< message;                   \
		tk::cliCore::log(ss.str());                   \
	} while (0)


#define LOG_WRN(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[WRN] | "<< message;                   \
		tk::cliCore::log(ss.str());                   \
	} while (0)


#define LOG_ERR(message)                              \
	do                                                \
	{                                                 \
		std::stringstream ss;                         \
		ss << "[ERR] | "<< message;                   \
		tk::cliCore::log(ss.str());                   \
	} while (0)

// clang-format on