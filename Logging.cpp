#include "Logging.h"

const char* LogLevels[] =
{
	"NOTICE",
	"WARNING",
	"ERROR"
};

int LOG(std::string text, LOG_LEVEL level) 
{
	std::cerr << LogLevels[level] << ": " << text << std::endl;
	return level == ERROR ? -1 : 0;
}

int LOG_NOTICE(std::string text)
{
	return LOG(text, NOTICE);
}

int LOG_WARNING(std::string text)
{
	return LOG(text, WARNING);
}

int LOG_ERROR(std::string text) 
{
	return LOG(text, ERROR);
}