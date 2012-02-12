//////////////////////////////////////////////
//	Filename: Client.h
//////////////////////////////////////////////

#ifndef _CLIENT_H_
#define _CLIENT_H_

//////////////////////////////////////////////
//	Includes
//////////////////////////////////////////////

#include <Windows.h>

//////////////////////////////////////////////
//	Class name: Client
//////////////////////////////////////////////

class Client {
private:
	SOCKET _clientSocket;
	HANDLE _clientThread;
	int _clientPort;
	char* _clientAddress;
	int _backlog;
	bool _continue;
public:
	__event void Connect();
	__event void Error(char* function, int errorCode);
	__event void Start();
	__event void Stop();
	Client(char* clientAddress, int clientPort);
	Client(Client& rClient);
	~Client();
	void LaunchclientRoutine();
	void StopclientRoutine();
	bool Isclienting();
private:
	DWORD clientRoutine();
	static DWORD ThreadLauncher(LPVOID routineParams);
};

#endif