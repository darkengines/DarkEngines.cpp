//////////////////////////////////////////////
//	Filename: Listener.h
//////////////////////////////////////////////

#ifndef _LISTENER_H_
#define _LISTENER_H_

//////////////////////////////////////////////
//	Includes
//////////////////////////////////////////////

#include <Windows.h>

//////////////////////////////////////////////
//	Class name: Listener
//////////////////////////////////////////////

class Listener {
private:
	SOCKET _listenSocket;
	int _listenPort;
	char* _listenAddress;
	int _backlog;
	bool _continue;
public:
	__event void Accept(SOCKET socket);
	__event void Error(char* function, int errorCode);
	__event void Start();
	__event void Stop();
	Listener(char* listenAddress, int listenPort);
	void LaunchListenRoutine();
private:
	DWORD ListenRoutine();
	static DWORD ThreadLauncher(LPVOID routineParams);
};

#endif