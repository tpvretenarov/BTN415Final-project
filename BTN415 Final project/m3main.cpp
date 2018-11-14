#include "Pkt_Def.h"
#include "MySocket.h"
#include <iostream> 
#include <string>
#include <iomanip>
#include <thread>
#pragma comment ( lib , "Ws2_32.lib" ) 

using namespace std;

bool ExeComplete = false;


void commandThread(std::string ip, int port)
{
	MySocket command(SocketType::CLIENT, ip, port, ConnectionType::TCP, 100);
	command.ConnectTCP();
	int packetAmount = 0;

	while (ExeComplete == false)
	{
		char* sendPacket;
		PktDef cmdPacket, sendAck;
		char cmd = ' ';
		cmdPacket = PktDef();

		std::cout << "Enter D for Drive, A for Arm, C for Claw, or X for Sleep!" << std::endl;
		std::cin >> cmd;
		std::cin.ignore();

		if (cmd == 'D')
		{
			packetAmount++;
			cmdPacket.SetPktCount(packetAmount);
			cmdPacket.SetCmd(DRIVE);

			int dur = 0, dir = 0;
			std::cout << "Enter 1 for forward, 2 for backward, 3 for right, 4 for left!" << std::endl;
			std::cin >> dir;
			std::cout << "Enter how long in seconds!" << std::endl;
			std::cin >> dur;
			std::cin.ignore();

			char* body = new char[2];

			body[0] = (char)dir;
			body[1] = (char)dur;

			//send packet
			cmdPacket.SetBodyData(body, 2);
			cmdPacket.CalcCRC();

			sendPacket = new char[cmdPacket.GetLength()];
			sendPacket = cmdPacket.GenPacket();

			//PktDef tempPacket = PktDef(sendPacket);

			command.SendData(sendPacket, cmdPacket.GetLength());

			//receive ack
			char* ackBuffer = new char[128];

			//char ackHead[7];
			//Header headA;
			command.GetData(ackBuffer);
			PktDef ackP = PktDef(ackBuffer);

			//memcpy(&headA, ackBuffer, sizeof(Header));

			if (ackP.GetCmd() == DRIVE)
			{
				std::cout << "Drive Packet successfully received!" << std::endl;
			}

			else
			{
				std::cout << "Drive Packet was not received!" << std::endl;
			}
		}

		else if (cmd == 'A') {
			packetAmount++;
			cmdPacket.SetPktCount(packetAmount);
			cmdPacket.SetCmd(ARM);

			int dur = 0, dir;
			std::cout << "Enter 5 to move arm up, 6 to move arm down!" << std::endl;
			std::cin >> dir;
			std::cin.ignore();

			char* body = new char[2];

			body[0] = (char)dir;
			body[1] = (char)dur;

			cmdPacket.SetBodyData(body, 2);
			cmdPacket.CalcCRC();

			sendPacket = new char[cmdPacket.GetLength()];
			sendPacket = cmdPacket.GenPacket();
			command.SendData(sendPacket, cmdPacket.GetLength());

			//receive ack
			char* ackBuffer = new char[128];

			//char ackHead[7];
			//Header headA;
			command.GetData(ackBuffer);
			PktDef ackP = PktDef(ackBuffer);

			//memcpy(&headA, ackBuffer, sizeof(Header));

			if (ackP.GetCmd() == ARM)
			{
				std::cout << "Arm Packet successfully received!" << std::endl;
			}

			else
			{
				std::cout << "Arm Packet was not received!" << std::endl;
			}
		}
		else if (cmd == 'C') {
			packetAmount++;
			cmdPacket.SetPktCount(packetAmount);
			cmdPacket.SetCmd(CLAW);

			int dir, dur = 0;
			std::cout << "Enter 7 to open the claw, 8 to close the claw!" << std::endl;
			std::cin >> dir;
			std::cin.ignore();

			char* body = new char[2];
			body[0] = (char)dir;
			body[1] = (char)dur;

			cmdPacket.SetBodyData(body, 2);
			cmdPacket.CalcCRC();

			sendPacket = new char[cmdPacket.GetLength()];
			sendPacket = cmdPacket.GenPacket();
			command.SendData(sendPacket, cmdPacket.GetLength());

			//receive ack
			char* ackBuffer = new char[128];

			//char ackHead[7];
			//Header headA;
			command.GetData(ackBuffer);
			PktDef ackP = PktDef(ackBuffer);

			//memcpy(&headA, ackBuffer, sizeof(Header));

			if (ackP.GetCmd() == CLAW)
			{
				std::cout << "Claw Packet successfully received!" << std::endl;
			}

			else
			{
				std::cout << "Claw Packet was not received!" << std::endl;
			}
		}
		else if (cmd == 'X') {
			packetAmount++;
			cmdPacket.SetPktCount(packetAmount);
			cmdPacket.SetCmd(SLEEP);
			cmdPacket.CalcCRC();

			sendPacket = new char[cmdPacket.GetLength()];
			sendPacket = cmdPacket.GenPacket();
			command.SendData(sendPacket, cmdPacket.GetLength());

			//receive ack
			char* ackBuffer = new char[128];

			//char ackHead[7];
			//Header headA;
			command.GetData(ackBuffer);
			PktDef ackP(ackBuffer);

			//memcpy(&headA, ackBuffer, sizeof(Header));

			ExeComplete = true;

			if (ackP.GetCmd() == SLEEP)
			{
				std::cout << "Sleep Packet successfully received!" << std::endl;
			}

			else
			{
				std::cout << "Sleep Packet was not received!" << std::endl;
			}
		}

		else
		{
			std::cout << "Please re-enter!" << std::endl;
			continue;
		}
	}
}

/*
void telemetryThread(std::string ip, int port)
{
	MySocket telemetry(SocketType::CLIENT, ip, port, ConnectionType::TCP, 100);
	telemetry.ConnectTCP();

	char* telBuffer = new char[128];

	while (ExeComplete == false)
	{

		int telSize = telemetry.GetData(telBuffer);
		PktDef rxPkt(telBuffer);

		std::cout << "*************Telemetry Socket Data*************" << endl;

		if (rxPkt.CheckCRC(telBuffer, telSize) == true)
		{
			if (rxPkt.GetCmd() == STATUS)
			{
				unsigned char* raw = (unsigned char *)telBuffer;

				for (int x = 0; x < telSize; x++)
				{
					std::cout << std::hex << std::setw(4) << (unsigned int*)(raw++) << ", ";
					std::cout << std::endl;
				}

			}
		}

		char* telBod = new char[5];

		telBod = rxPkt.GetBodyData();

		std::cout << "Sonar Reading: " << std::dec << (unsigned int)*(telBod++);
		std::cout << std::endl;

		std::cout << "Arm Position:" << std::dec << (unsigned int)*(telBod++);
		std::cout << std::endl;

		char * driveBit = (char*)telBod;
		std::cout << "Drive Bit: " << (bool)((*driveBit) & 0x01) << std::endl;

		if ((*driveBit >> 1) & 0x01)
		{
			std::cout << "Arm is Up" << std::endl;
		}

		else if ((*driveBit >> 2) & 0x01)
		{
			std::cout << "Arm is Down" << std::endl;
		}

		if ((*driveBit >> 3) & 0x01)
		{
			std::cout << "Claw is Open" << std::endl;
		}

		else if ((*driveBit >> 4) & 0x01) {
			std::cout << "Claw is Closed" << std::endl << std::endl;
		}
	}
}
*/

void main() {

		std::string ip;
		unsigned int cport, tport;

		std::cout << "Please enter an IP address to connect to!" << std::endl;
		std::cin >> ip;
		std::cout << "Please enter the C port to connect to!" << std::endl;
		std::cin >> cport;
		//std::cout << "Please enter the T port to connect to!" << std::endl;
		//std::cin >> tport;
		
		std::thread(commandThread, ip, cport).detach();
		//std::thread(telemetryThread, ip, tport).detach();
		while (!ExeComplete);
}