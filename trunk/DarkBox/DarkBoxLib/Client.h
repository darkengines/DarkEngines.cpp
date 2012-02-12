//////////////////////////////////////////////
//	Filename: Client.h
//////////////////////////////////////////////

#ifndef _CLIENT_H_
#define _CLIENT_H_

//////////////////////////////////////////////
//	Includes
//////////////////////////////////////////////

#include <Windows.h>

#define BUFFER_SIZE 2048

//////////////////////////////////////////////
//	Class name: Client
//////////////////////////////////////////////

class Client {
private:
	SOCKET _clientSocket;
	HANDLE _clientThread;
public:
	Client(SOCKET socket);
	int SendSizeHeader(int size);
	int ReceiveSizeHeader(int* size);
	int SendBytes(void* bytes, int size);
	int ReceiveBytes(void* bytes);
	int SendInt(int value);
	int ReceiveInt(int* value);
	template<typename T>
	int Send(T value);
	template<typename T>
	int Receive(T* value);
private:
};

#endif