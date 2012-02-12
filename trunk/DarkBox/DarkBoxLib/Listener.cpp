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
Listener::Listener(Listener& rListener) {
	
}
Listener::~Listener() {

}

void Listener::LaunchListenRoutine()  {
	_continue = true;
	_listenThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadLauncher, (LPVOID)this, 0, 0);
	if (_listenThread == 0) {
		Error("CreateThread", GetLastError());
	}
	Start();
}

DWORD Listener::ListenRoutine() {
	_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_listenSocket == INVALID_SOCKET) {
		Error("socket", WSAGetLastError());
		Stop();
		return 1;
	}
	hostent* host = gethostbyname(_listenAddress);
	if (!host) {
		closesocket(_listenSocket);
		Error("gethostbyname", WSAGetLastError());
		Stop();
		return 1;
	}
	char* IP = inet_ntoa (*(struct in_addr *)*host->h_addr_list);
	struct sockaddr_in socketInfos;
	socketInfos.sin_family = AF_INET;
	socketInfos.sin_addr.s_addr = inet_addr(IP);
	socketInfos.sin_port = htons(_listenPort);
	int error = bind(_listenSocket, (SOCKADDR*)&socketInfos, sizeof(socketInfos));
	if (error == SOCKET_ERROR) {
		closesocket(_listenSocket);
		Error("bind", WSAGetLastError());
		Stop();
		return 1;
	}
	error = listen(_listenSocket, _backlog);
	if (error == SOCKET_ERROR) {
		closesocket(_listenSocket);
		Error("listen", WSAGetLastError());
		Stop();
		return 1;
	}
	SOCKET socket = 0;
	while (_continue) {
		socket = accept(_listenSocket, 0, 0);
		if (socket == INVALID_SOCKET) {
			if (error != 0) {
				Error("accept", WSAGetLastError());
				closesocket(_listenSocket);
				Stop();
				return 1;
			}
		} else {
			Accept(socket);
		}
	}
	closesocket(_listenSocket);
	Stop();
	return 0;
}

DWORD Listener::ThreadLauncher(LPVOID routineParams) {
	return ((Listener*)routineParams)->ListenRoutine();
}

void Listener::StopListenRoutine() {
	_continue = false;
	closesocket(_listenSocket);
	WaitForSingleObject(_listenThread, INFINITE);
}

bool Listener::IsListening() {
	return _continue;
}