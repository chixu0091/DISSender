
#include "Connection.h"
#include "DataManager.h"
#include "DISdata.h"

#include "Logging.h"


// ./DISSender.exe [filepath] [time_interval] [max_sendings]
int main(int argc, char* args[])
{
	// new udp connection instance and data manager
	Example::UDPConnection conn;
	Example::DataManager dm;

	// file path to be read
	std::string path("");
	// time interval for one round of DIS packets send
	uin time_interval(0);
	uin max_sendings(0);
	uin count(0);

	for (int i = 0; i < argc; ++i) {
		switch (i) {
		case 0:
			break;
		case 1:
			path = args[i];
			break;
		case 2:
			time_interval = std::atoi(args[i]);
			break;
		case 3:
			max_sendings = std::atoi(args[i]);
			break;
		default:
			LOG_ERROR("Parm Num > 4, USE \n .\\x64\\Debug\DISSender.exe [filepath] [time_interval][max_sendings] i.e. \n .\\x64\\Debug\DISSender.exe .\\testdata\\ 1 10");
			return 0;
		}
	}

	// set path to "./testdata" default
	if (path.empty())
		path = "./testdata/";
	// set time range to 1000ms default
	if (!time_interval)
		time_interval = 1000;
	// set max sendings to 10 times default
	if (!max_sendings)
		max_sendings = 10;
	printf("***curr data is %s \n***time interval is %d \n***max number of sendings is %d \n", path.c_str(), time_interval, max_sendings);

	conn.Connect(PORT, IP, false);
	dm.Init(path);

	unsigned time = SDL_GetTicks();

	while (count < max_sendings) {
		// TODO : add signal handling https://docs.microsoft.com/en-us/windows/console/setconsolectrlhandler
		if (count >= max_sendings) {
			break;
		}
		unsigned now = SDL_GetTicks();
		unsigned delta_time = now - time;
		if (delta_time > time_interval) {
			printf("The time is %dms %d\n", SDL_GetTicks(), count);
			dm.SendAllFiles(&conn);
			dm.UpdateRawIndexOnce();
			time = now;
			++count;
		}
	}

	conn.Disconnect();

	return 0;
}