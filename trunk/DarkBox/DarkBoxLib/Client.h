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
	int RoutineCode;
} RoutineParams;

//////////////////////////////////////////////
//	Class name: Client
//////////////////////////////////////////////

class Client {
private:
	SOCKET _clientSocket;
	HANDLE _clientThread;
	bool _continue;
public:
	__event void Connect(Client* client);
	__event void Disconnect();
	__event void Error(char* function, int errorCode);
	__event void Authentification(Client* client, void* authentificator);
	__event void Identification(Client* client, void* identificator);
	__event void Command(char* command);
	__event void Progress( int progress);
	__event void SizeHeader(int size);
	Client(SOCKET socket);
	int Connect(char* address, int port);
	int SendSizeHeader(int size);
	int ReceiveSizeHeader(int* size);
	template<typename T>
	int Send(T* pValue, int count, bool fireProgress) {
		return SendBytes(pValue, sizeof(T)*count, fireProgress);
	}
	template<typename T>
	int Receive(T* pValue, int count, bool fireProgress) {
		return ReceiveBytes(pValue, sizeof(T)*count, fireProgress);
	}
	int SendString(char* string);
	int ReceiveString(char** string);
	int GetAddress(char* address);
	int GetPort(int* port);
	int GetAddressStringLength(int* length);
	int LaunchCommandRoutine();
private:
	int GetByteStringLength(unsigned char byte);
	int SendBytes(void* bytes, int size, bool fireProgress);
	int ReceiveBytes(void* bytes, bool fireProgress);
	DWORD CommandRoutine();
	DWORD UploadRoutine();
	DWORD DownloadRoutine();
	static DWORD ThreadLauncher(LPVOID routineParams);
};

#endif