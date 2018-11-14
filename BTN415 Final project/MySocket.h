#ifndef SOCKET_H
#define SOCKET_H

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <thread>
#include <bitset>

enum SocketType{ CLIENT, SERVER };
enum ConnectionType { TCP, UDP };

const int DEFAULT_SIZE = 128;

class MySocket {

private:
	char *buffer;
	SOCKET WelcomeSocket;
	SOCKET ConnectionSocket;
	sockaddr_in SvrAddr;
	SocketType mySocket;
	std::string IPAddr;
	int Port;
	ConnectionType connectionType;
	bool bTCPConnect;
	int MaxSize;

	WSADATA wsa_data;
	int version_num1, version_num2;

public:
	MySocket(SocketType, std::string, unsigned int, ConnectionType, unsigned int);
	~MySocket();
	void ConnectTCP();
	void DisconnectTCP();
	void SendData(const char*, int);
	int GetData(char*);
	std::string GetIPAddr();
	void SetIPAddr(std::string);
	void SetPortS(int);
	int GetPort();
	SocketType GetType();
	void SetType(SocketType);
};

#endif