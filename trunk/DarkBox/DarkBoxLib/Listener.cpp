//////////////////////////////////////////////
//	Filename: Listener.cpp
//////////////////////////////////////////////

#include "Listener.h"

Listener::Listener(char* listenAddress, int listenPort) {
	_listenAddress = listenAddress;
	_listenPort = listenPort;
	_backlog = 256;
	_continue = false;
}

void Listener::LaunchListenRoutine()  {
	HANDLE listenThread;
	_continue = true;
	listenThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadLauncher, (LPVOID)this, 0, 0);
}

DWORD Listener::ListenRoutine() {
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	hostent* host = gethostbyname(_listenAddress);
	char* IP = inet_ntoa (*(struct in_addr *)*host->h_addr_list);
	struct sockaddr_in socketInfos;
	socketInfos.sin_family = AF_INET;
	socketInfos.sin_addr.s_addr = inet_addr(IP);
	socketInfos.sin_port = htons(_listenPort);
	int result = bind(_listenSocket, (SOCKADDR*)&socketInfos, sizeof(socketInfos));
	result = listen(_listenSocket, _backlog);
	SOCKET socket = 0;
	while (_continue) {
		socket = accept(_listenSocket, 0, 0);
		Accept(socket);
	}
	return 0;
}

DWORD Listener::ThreadLauncher(LPVOID routineParams) {
	return ((Listener*)routineParams)->ListenRoutine();
}