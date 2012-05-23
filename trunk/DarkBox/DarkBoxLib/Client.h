//////////////////////////////////////////////
//	Filename: Client.h
//////////////////////////////////////////////

#ifndef _CLIENT_H_
#define _CLIENT_H_

//////////////////////////////////////////////
//	Includes
//////////////////////////////////////////////

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048

//////////////////////////////////////////////
//	Typedefs
//////////////////////////////////////////////

class Client;

typedef struct {
	Client* owner;
	void* params;
} RoutineParams;

//////////////////////////////////////////////
//	Class name: Client
//////////////////////////////////////////////

class Client {
private:
	HANDLE _clientThread;
	bool _continue;
public:
SOCKET _clientSocket;
	__event void ConnectEvent(Client* client);
	Client(SOCKET socket);
	~Client();
	int Connect(char* address, int port);
	int Disconnect();
	int SendSizeHeader(int size);
	int ReceiveSizeHeader(int* size);
	template<typename T>
	int Send(T* pValue, int count) {
		return SendBytes(pValue, sizeof(T)*count);
	}
	template<typename T>
	int Receive(T* pValue, int count) {
		return ReceiveBytes(pValue);
	}
	int SendString(char* string);
	int ReceiveString(char** string);
	int GetAddress(char* address);
	int GetPort(int* port);
	int GetAddressStringLength(int* length);
	int LaunchRoutine(DWORD(*Routine)(RoutineParams*), RoutineParams* routineParams);
private:
	int GetByteStringLength(unsigned char byte);
	int SendBytes(void* bytes, int size);
	int ReceiveBytes(void* bytes);
};

#endif