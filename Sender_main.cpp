
#include "Connection.h"
#include "DataManager.h"
#include "DISdata.h"

#include "Logging.h"

int main(int argc, char* args[])
{
	(void)argc;
	(void)args;

	// new udp connection instance
	Example::UDPConnection conn;
	Example::DataManager dm;
	

	conn.Connect(PORT, IP, false);
	
	//dm.Init("./testdata");
	dm.Init();
	//dm.PrintPathsTypes();
	//dm.SendFileDisData(&conn, dm.GetFile(0));
	dm.SendAllFiles(&conn);
	dm.UpdateRawIndexOnce();
	dm.SendAllFiles(&conn);

	conn.Disconnect();

	return 0;
}