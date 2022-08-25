#pragma once

#include <iostream>
#include <string>

enum LOG_LEVEL
{
	NOTICE = 0,
	WARNING,
	ERROR
};

int LOG(std::string text, LOG_LEVEL level);

int LOG_NOTICE(std::string text);
int LOG_WARNING(std::string text);
int LOG_ERROR(std::string text);

