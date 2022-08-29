#pragma once

#include <string>
#include <iostream>

enum LOG_LEVEL
{
	NOTICE = 0,
	WARNING,
	ERROR
};
// TODO: update LOG to c-printf style
// https://stackoverflow.com/questions/68154231/how-do-i-define-a-function-that-accepts-a-formatted-input-string-in-c
int LOG(std::string text, LOG_LEVEL level);

int LOG_NOTICE(std::string text);
int LOG_WARNING(std::string text);
int LOG_ERROR(std::string text);

