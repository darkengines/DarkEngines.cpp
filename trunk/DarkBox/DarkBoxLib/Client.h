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

#define BUFFER_SIZE 2048

//////////////////////////////////////////////
//	Class name: Client
//////////////////////////////////////////////

class Client {
private:
	SOCKET _clientSocket;
	HANDLE _clientThread;
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
	int Send(T* pValue, int count, bool fireProgress);
	template<typename T>
	int Receive(T* pValue, int count, bool fireProgress);
	int GetAddress(char* address);
	int GetPort(int* port);
	int GetAddressStringLength(int* length);
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