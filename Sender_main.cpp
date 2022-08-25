
#include "Connection.h"
#include "DataManager.h"
#include "DISdata.h"

#include "Logging.h"
#include <iostream>

const unsigned int PORT = 3000;
const std::string IP("127.0.0.1");


int main(int argc, char* args[])
{
	(void)argc;
	(void)args;

	// new udp connection instance
	Example::UDPConnection conn;
	Example::DataManager dm;
	

	conn.Connect(PORT, IP, false);
	
	
	//dm.PrintALL();
	//dm.ReadFile(".\\data\\I1.txt");
	//dm.PrintFile(".\\data\\I1.txt");
	
	dm.Init("./testdata");
	//dm.PrintPathsTypes();
	dm.SendAllFiles(&conn);
	//ush site, ush application, ush ID, DATA_TYPE t, Example::DeadReckoningModel p
	/*Example::DISData dd(0, 1, 0, MISSILE, Example::STATIC);
	Example::DISData dd2(0, 1, 1, MISSILE, Example::STATIC);

	dd.marshal(buffer);
	conn.Send(&buffer[0], buffer.size());
	buffer.clear();
	dd2.marshal(buffer);
	
	conn.Send(&buffer[0], buffer.size());
	buffer.clear();
	*/



	conn.Disconnect();

	return 0;
}