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
	__event void Connect();
	__event void Disconnect();
	__event void Error(char* function, int errorCode);
	__event void Authentification(Client client, void* authentificator);
	__event void Identification(Client client, void* identificator);
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
private:
	int SendBytes(void* bytes, int size, bool fireProgress);
	int ReceiveBytes(void* bytes, bool fireProgress);
	DWORD CommandRoutine();
	DWORD UploadRoutine();
	DWORD DownloadRoutine();
	static DWORD ThreadLauncher(LPVOID routineParams);
};

#endif