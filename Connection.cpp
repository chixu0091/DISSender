#include "Connection.h"
#include "Logging.h"

#include <sstream>
#include <cstring>

#include <SDL.h>
#include <SDL_net.h>

using namespace Example;

int UDPConnection::Connect(unsigned int port, const std::string& host, bool listen)
{
	if (SDL_Init(0) == -1 || SDLNet_Init() == -1) {
		return LOG_ERROR("UDP coonection init FAIL");
	}

	if (SDLNet_ResolveHost(&mAddr, host.c_str(), port) == -1) {
		std::ostringstream strm;
		strm << "Can't get address for : " + host + ". "
			<< "Error:" << SDLNet_GetError()
			<< "\n. System: " << SDL_GetError();
		return LOG_ERROR(strm.str());
	}

	if (listen) {
		mSocket = SDLNet_UDP_Open(port);
	}
	else {
		mSocket = SDLNet_UDP_Open(0);
	}

	if (!mSocket) {
		std::ostringstream strm;
		strm << "Can't open socket: " << SDLNet_GetError();
		LOG_ERROR(strm.str());
	}

	return 0;
}

int UDPConnection::Disconnect() 
{
	SDLNet_UDP_Close(mSocket);
	SDLNet_Quit();
	return 0;
}

int UDPConnection::Send(const char* buf, size_t numbytes)
{
	if (numbytes < 1)
		return -1;

	UDPpacket* packet = SDLNet_AllocPacket(numbytes);
	if (!packet) {
		return LOG_ERROR("Alloc packet FAIL");
	}
	else {
		memcpy(packet->data, buf, numbytes);
		packet->address = mAddr;
		packet->len = numbytes;

		if (!SDLNet_UDP_Send(mSocket, -1, packet)) {
			SDLNet_FreePacket(packet);
			return LOG_ERROR("Send packet FAIL");
		}
		SDLNet_FreePacket(packet);
		return 0;

	}
}

size_t UDPConnection::Receive(char* buf)
{
	UDPpacket* packet = SDLNet_AllocPacket(MTU_SIZE);
	int packetRecvd = SDLNet_UDP_Recv(mSocket, packet);

	if (packetRecvd != 1){
		if (packetRecvd == -1) {
			LOG_ERROR("Recv packet FAIL");
		}
		// zero len packet received
		SDLNet_FreePacket(packet);
		return 0;
	}
	memcpy(buf, packet->data, packet->len);
	SDLNet_FreePacket(packet);
	return packet->len;
}


















































