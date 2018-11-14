	#include <windows.networking.sockets.h>
	#pragma comment(lib, "Ws2_32.lib")
	#include <iostream>
	#include <thread>
	#include <bitset>
	#include "MySocket.h"


	MySocket::MySocket(SocketType sType, std::string ipAdd, unsigned int portNum, ConnectionType cType, unsigned int size) {

		this->version_num1 = 2;
		this->version_num2 = 2;
	
		if ((WSAStartup(MAKEWORD(this->version_num1, this->version_num2), &this->wsa_data)) != 0) {
			std::cout << "Could not start DLLs" << std::endl;
			std::cin.get();
			//exit(0);
		}


		if (cType == 0)   //TCP
		{
			if (sType == 0)  //TCP, CLIENT
			{
				mySocket = sType;

				this->Port = portNum;
				this->IPAddr = ipAdd;

				SOCKET LocalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				if (LocalSocket == INVALID_SOCKET)
				{
					WSACleanup();
					std::cout << "Could not intialize TCP Client Socket!!!!!!!" << std::endl;
					std::cin.get();
					//exit(0);
				}

				this->ConnectionSocket = LocalSocket;
			}

			else if (sType == 1)  //TCP, SERVER
			{
				mySocket = sType;

				this->Port = portNum;
				this->IPAddr = ipAdd;

				SOCKET LocalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				if (LocalSocket == INVALID_SOCKET) 
				{
					WSACleanup();
					std::cout << "Could not intialize TCP Server Socket!!!!!!!!" << std::endl;
					std::cin.get();
					//exit(0);
				}

				this->WelcomeSocket = LocalSocket;

				//BIND
				SvrAddr.sin_family = AF_INET; //Address family type internet
				SvrAddr.sin_port = htons(this->Port); //port (host to network conversion)
				SvrAddr.sin_addr.s_addr = inet_addr(this->IPAddr.c_str()); //IP address
				if ((bind(this->WelcomeSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
					closesocket(this->WelcomeSocket);
					WSACleanup();
					std::cout << "Could not bind to the socket" << std::endl;
					std::cin.get();
					exit(0);
				}

				//LISTEN
				if (listen(this->WelcomeSocket, 1) == SOCKET_ERROR) {
					closesocket(this->WelcomeSocket);
					WSACleanup();
					std::cout << "Could not listen to the provided socket." << std::endl;
					std::cin.get();
					exit(0);
				}
				else {
					std::cout << "Waiting for client connection" << std::endl;
				}

				//ACCEPT
				if ((this->ConnectionSocket = accept(this->WelcomeSocket, NULL, NULL)) == SOCKET_ERROR) {
					closesocket(this->WelcomeSocket);
					WSACleanup();
					std::cout << "Could not accept incoming connection." << std::endl;
					std::cin.get();
					exit(0);
				}
				else {
					std::cout << "Connection Accepted" << std::endl;
				}
			}
		}

		else if (cType == 1)  //UDP
		{
			if (sType == 0)  //UDP, CLIENT
			{
				mySocket = sType;

				this->Port = portNum;
				this->IPAddr = ipAdd;

				SOCKET LocalSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

				if (LocalSocket == INVALID_SOCKET)
				{
					WSACleanup();
					std::cout << "Could not intialize UDP Client Socket!!!!!!" << std::endl;
					std::cin.get();
					//exit(0);
				}

				this->ConnectionSocket = LocalSocket;
			}

			else if (sType == 1)  //UDP, SERVER
			{
				mySocket = sType;

				this->Port = portNum;
				this->IPAddr = ipAdd;

				SOCKET LocalSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

				if (LocalSocket == INVALID_SOCKET)
				{
					WSACleanup();
					std::cout << "Could not intialize UDP Server Socket!!!!!!" << std::endl;
					std::cin.get();
					//exit(0);

				}

				this->WelcomeSocket = LocalSocket;

				//BIND
				SvrAddr.sin_family = AF_INET; //Address family type internet
				SvrAddr.sin_port = htons(this->Port); //port (host to network conversion)
				SvrAddr.sin_addr.s_addr = inet_addr(this->IPAddr.c_str()); //IP address
				if ((bind(this->WelcomeSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
					closesocket(this->WelcomeSocket);
					WSACleanup();
					std::cout << "Could not bind to the socket" << std::endl;
					std::cin.get();
					exit(0);
				}

				//LISTEN
				if (listen(this->WelcomeSocket, 1) == SOCKET_ERROR) {
					closesocket(this->WelcomeSocket);
					WSACleanup();
					std::cout << "Could not listen to the provided socket." << std::endl;
					std::cin.get();
					exit(0);
				}
				else {
					std::cout << "Waiting for client connection" << std::endl;
				}

				//ACCEPT
				if ((this->ConnectionSocket = accept(this->WelcomeSocket, NULL, NULL)) == SOCKET_ERROR) {
					closesocket(this->WelcomeSocket);
					WSACleanup();
					std::cout << "Could not accept incoming connection." << std::endl;
					std::cin.get();
					exit(0);
				}
				else {
					std::cout << "Connection Accepted" << std::endl;
				}
			}
		}

		if (size > 128) 
		{
			size = DEFAULT_SIZE;
		}

		MaxSize = size;
	}

	MySocket::~MySocket() 
	{
		delete[] buffer;
		closesocket(this->ConnectionSocket);
		closesocket(this->WelcomeSocket);
	}

	void MySocket::ConnectTCP()
	{
		if (mySocket == 0)
		{
			SvrAddr.sin_family = AF_INET;
			SvrAddr.sin_port = htons(this->Port);
			SvrAddr.sin_addr.s_addr = inet_addr(this->IPAddr.c_str());
			if ((connect(this->ConnectionSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
			{
				closesocket(this->ConnectionSocket);
				WSACleanup();

				std::cout << "Could not connect to the TCP server!!!!!!" << std::endl;
			
			}
		}

		else if (mySocket == 1)
		{
			std::cout << "Cannot connect Server to Server!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		}
	}

	void MySocket::DisconnectTCP()
	{
		closesocket(this->ConnectionSocket);
	}

	void MySocket::SendData(const char* d, int nb)
	{
		if (mySocket == 0)
		{
			send(this->ConnectionSocket, d, nb, 0);
		}

		else if (mySocket == 1)
		{
			sendto(this->ConnectionSocket, d, nb, 0, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr));
		}
	}

	int MySocket::GetData(char* d)
	{
		int count = 0;

		if (mySocket == 0)
		{
			buffer = new char[MaxSize];

			recv(this->ConnectionSocket, buffer, MaxSize, 0);

			for (int i = 0; i < MaxSize; i++)
			{
				d[i] = buffer[i];
				count++;

				if (buffer[i] == '\0') 
				{
					d[i] = buffer[i];
					break;
				}
			}
		}

		else if (mySocket == 1)
		{
			int addr_len;
			addr_len = sizeof(SvrAddr);

			buffer = new char[MaxSize];

			recvfrom(this->ConnectionSocket, buffer, MaxSize, 0, (struct sockaddr *)&SvrAddr, &addr_len);


			for (int i = 0; i < MaxSize; i++) 
			{
				d[i] = buffer[i];
				count++;

				if (buffer[i] == '\0') 
				{
					d[i] = buffer[i];
					break;
				}
			}
		}
		return count;
	}

	std::string MySocket::GetIPAddr()
	{
		return IPAddr;
	}

	void MySocket::SetIPAddr(std::string ip)
	{
		if (this->ConnectionSocket != NULL)
		{
			IPAddr = ip;
		}

		else
		{
			std::cout << "Connection has already been established!!!!!!" << std::endl;
		}
	}

	int MySocket::GetPort()
	{
		return Port;
	}

	void MySocket::SetPortS(int p)
	{
		if (this->ConnectionSocket != NULL)
		{
			Port = p;
		}

		else
		{
			std::cout << "Connection has already been established!!!!!!" << std::endl;
		}
	}

	SocketType MySocket::GetType()
	{
		return mySocket;
	}

	void MySocket::SetType(SocketType st)
	{
		if (this->ConnectionSocket != NULL)
		{
			mySocket = st;
		}

		else
		{
			std::cout << "Connection has already been established!!!!!!" << std::endl;
		}
	}
