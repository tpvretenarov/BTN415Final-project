//This project was done by: Jordan, Todor, Anujan

#include "Pkt_Def.h"
#include <iostream>
#include <sstream>
#include <bitset>
#include <sstream>
#include <bitset>


PktDef::PktDef()
{
	packet.head.PktCount = 0;
	packet.head.Drive = 0;
	packet.head.Status = 0;
	packet.head.Sleep = 0;
	packet.head.Arm = 0;
	packet.head.Claw = 0;
	packet.head.Ack = 0;
	packet.head.padding = 0;
	packet.head.Length = 7;

	packet.data = nullptr;

	packet.CRC = 0x00;

	RawBuffer = nullptr;
}

PktDef::PktDef(char* raw)
{
	memcpy(&packet.head, raw, sizeof(Header));
	packet.data = new char[packet.head.Length - (sizeof(char)+sizeof(Header))];
	memcpy(packet.data, raw + sizeof(Header), packet.head.Length - (sizeof(char) + sizeof(Header)));
	memcpy(&packet.CRC, raw + sizeof(Header) + packet.head.Length - (sizeof(char) + sizeof(Header)), sizeof(char));
}

void PktDef::SetCmd(CmdType cmd)
{	
	if (cmd == CmdType::DRIVE)packet.head.Drive = (unsigned)1;
	if (cmd == CmdType::STATUS)packet.head.Status = (unsigned)1;
	if (cmd == CmdType::SLEEP)packet.head.Sleep = (unsigned)1;
	if (cmd == CmdType::ARM)packet.head.Arm = (unsigned)1;
	if (cmd == CmdType::CLAW)packet.head.Claw = (unsigned)1;
	if (cmd == CmdType::ACK)packet.head.Ack = (unsigned)1;
}

void PktDef::SetBodyData(char* cb, int set)
{
	packet.head.Length += (char)set;
	packet.data = new char[set];
	packet.data[0] = cb[0];
	packet.data[1] = cb[1];
}

void PktDef::SetPktCount(int countNum) {
	packet.head.PktCount = countNum;
}

CmdType PktDef::GetCmd() {
	if (packet.head.Drive == 1) {
		return DRIVE;
	}
	else if (packet.head.Status == 1) {
		return STATUS;
	}
	else if (packet.head.Sleep == 1) {
		return SLEEP;
	}
	else if (packet.head.Arm == 1) {
		return CLAW;
	}
	else if (packet.head.Claw == 1) {
		return ARM;
	}
	else if (packet.head.Ack == 1) {
		return ACK;
	}
	else {
		return ACK;
	}
}

bool PktDef::GetAck() {
	if (packet.head.Ack == 0) {
		return false;
	}
	else {
		return true;
	}
}

int PktDef::GetLength() {
	return packet.head.Length;
}

char* PktDef::GetBodyData() {
	return packet.data;
}

int PktDef::GetPktCount() {
	return packet.head.PktCount;
}

bool PktDef::CheckCRC(char* buffer, int size) {
	char * bufftemp = new char[size];

	for (int i = 0; i < size; i++) {
		bufftemp[i] = buffer[i];
	}
	int crctemp = (int)bufftemp[8];
	int counter = 0;

	for (int x = 0; x < (size-1); x++) {
		for (int y = 0; y < 8; y++) {
			if ((bufftemp[x] & 0x01) == 0x01) {
				counter++;				
			}
			bufftemp[x] >>= 1;
		}
	}
	return crctemp == counter;
}

void PktDef::CalcCRC()
{
	int count = 0;
	unsigned int tempLength = packet.head.Length;
	unsigned int tempPktCount = packet.head.PktCount;
	unsigned int tempDirection = 0;
	unsigned int tempDuration = 0;
	bool dd = false;

	for (int a = 0; a < 32; a++)
	{
		if ((tempPktCount & 0x01) == 0x01) {
			count++;
		}
		tempPktCount >>= 1;
	}
	if ((packet.head.Drive & 0x01) == 0x01) {
		count++;
		tempDirection = packet.data[0];
		tempDuration = packet.data[1];
		dd = true;
	}
	if ((packet.head.Status & 0x01) == 0x01) {
		count++;
	}
	if ((packet.head.Sleep & 0x01) == 0x01) {
		count++;
	}
	if ((packet.head.Arm & 0x01) == 0x01) {
		count++;
		tempDirection = packet.data[0];
		tempDuration = packet.data[1];
		dd = true;
	}
	if ((packet.head.Claw & 0x01) == 0x01) {
		count++;
		tempDirection = packet.data[0];
		tempDuration = packet.data[1];
		dd = true;
	}
	if ((packet.head.Ack & 0x01) == 0x01) {
		count++;
		/*tempDirection = packet.data[0];
		tempDuration = packet.data[1];
		dd = true;*/
	}

	for (int x = 0; x < 8; x++) {
		if ((tempLength & 0x01) == 0x01) {
			count++;
		}
		tempLength >>= 1;
	}

	if (dd == true)
	{
		for (int x = 0; x < 8; x++) {
			if ((tempDirection & 0x01) == 0x01) {
				count++;
			}
			tempDirection >>= 1;
		}
		for (int x = 0; x < 8; x++) {
			if ((tempDuration & 0x01) == 0x01) {
				count++;
			}
			tempDuration >>= 1;
		}
	}
	packet.CRC = count;
}


char* PktDef::GenPacket() {

	RawBuffer = new char[9];
	
	unsigned char* buffer = (unsigned char*)&packet;

	for (int i = 0; i < sizeof(Header); i++) {
		RawBuffer[i] = (unsigned char)*(buffer++);
	}
	
	if ((packet.head.Drive & 0x01) == 0x01 || (packet.head.Arm & 0x01) == 0x01 || (packet.head.Claw & 0x01) == 0x01 || (packet.head.Ack & 0x01) == 0x01)
	{
		RawBuffer[6] = packet.data[0];
		RawBuffer[7] = packet.data[1];
		RawBuffer[8] = packet.CRC;
	}

	else
	{
		RawBuffer[6] = (char)0;
		RawBuffer[7] = (char)0;
		RawBuffer[8] = packet.CRC;
	}

	return RawBuffer;
}