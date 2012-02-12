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
	HANDLE _listenThread;
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
	Listener(Listener& rListener);
	~Listener();
	void LaunchListenRoutine();
	void StopListenRoutine();
	bool IsListening();
private:
	DWORD ListenRoutine();
	static DWORD ThreadLauncher(LPVOID routineParams);
};

#endif