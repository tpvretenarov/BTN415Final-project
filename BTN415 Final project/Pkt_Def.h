//This project was done by: Jordan, Todor, Anujan

#ifndef WINSOCK_H
#define WINSOCK_H

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <thread>
#include <bitset>

//Header packet
struct Header {
	unsigned int PktCount;
	unsigned Drive : 1;
	unsigned Status : 1;
	unsigned Sleep : 1;
	unsigned Arm : 1;
	unsigned Claw : 1;
	unsigned Ack : 1;
	unsigned padding : 2;
	unsigned char Length;
};
//Command packet
struct MotorBody {
	unsigned char Direction;
	unsigned char Duration;
};

//Validation packet
struct Tail {
	unsigned char CRC;
};

//Defines the command types
enum CmdType {
	DRIVE, STATUS, SLEEP, ARM, CLAW, ACK
};

//Drive command parameters
const int FORWARD = 1;
const int BACKWARD = 2;
const int RIGHT = 3;
const int LEFT = 4;

//Arm command parameters
const int UP = 5;
const int DOWN = 6;

//Claw command parameters
const int OPEN = 7;
const int CLOSE = 8;

//represents the size of the header in bytes, in this case 6 bytes .. 1 byte for flags and padding, 4 bytes for counter, 1 byte for char length
const int HEADERSIZE = 6;

class PktDef {
	struct CmdPacket {
		Header head;
		char * data;
		char CRC;
	};	
	CmdPacket packet;
	char * RawBuffer;

public:
	PktDef();
	PktDef(char*);

	void SetCmd(CmdType);
	void SetBodyData(char*, int);
	void SetPktCount(int);

	CmdType GetCmd();
	bool GetAck();
	int GetLength();
	char* GetBodyData();
	int GetPktCount();

	bool CheckCRC(char*, int);
	void CalcCRC();
	char* GenPacket();
};
#endif