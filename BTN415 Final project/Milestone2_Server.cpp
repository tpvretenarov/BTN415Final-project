#include <iostream>
#include "MySocket.h"
//#include "C:\Users\elliott.coleshill\Documents\AA_CURRICULUM_DEVELOPMENT\VEX Robotic Work\BeagleBone Integration\TCP_Server_Example\VEX_415_1701\MySocket.h"


int main()
{
	MySocket ServerSocket(SocketType::SERVER, "127.0.0.1", 5000, ConnectionType::TCP, 100);
	
	char buff[100];

	ServerSocket.ConnectTCP();

	int RxSize = ServerSocket.GetData(buff);
	
	std::cout << "Msg = " << buff << ", Bytes = " << RxSize << std::endl;
	
	std::string Pkt = "I Love BTN415 too!";
	
	ServerSocket.SendData(Pkt.c_str(), strlen(Pkt.c_str())+1);
	
	ServerSocket.DisconnectTCP();
	std::cin.get();
	return 1;
}