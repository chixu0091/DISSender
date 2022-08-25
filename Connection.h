#pragma once

#include <string>
#include <cstddef>

#include <SDL.h>
#include <SDL_net.h>

namespace Example
{
	//largest packet size
	const unsigned int MTU_SIZE = 1500;


	class UDPConnection
	{
	public:
		int Connect(unsigned int port, const std::string& host, bool listen);
		int Disconnect();

		int Send(const char* buf, size_t numbytes);
		size_t Receive(char* buf);

	private:
		UDPsocket mSocket;
		IPaddress mAddr;
	};
}