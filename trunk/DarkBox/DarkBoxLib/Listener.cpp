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
	if (listenThread == 0) {
		Error("CreateThread", GetLastError());
	}
}

DWORD Listener::ListenRoutine() {
	Start();
	WORD wVersionRequested;
    WSADATA wsaData;
    int error;
    wVersionRequested = MAKEWORD(2, 2);
	error = WSAStartup(wVersionRequested, &wsaData);
	if (error) {
		Error("WSAStartup", error);
		Stop();
		return 1;
	}
	_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_listenSocket == INVALID_SOCKET) {
		WSACleanup();
		Error("socket", WSAGetLastError());
		Stop();
		return 1;
	}
	hostent* host = gethostbyname(_listenAddress);
	if (!host) {
		closesocket(_listenSocket);
		WSACleanup();
		Error("gethostbyname", WSAGetLastError());
		Stop();
		return 1;
	}
	char* IP = inet_ntoa (*(struct in_addr *)*host->h_addr_list);
	struct sockaddr_in socketInfos;
	socketInfos.sin_family = AF_INET;
	socketInfos.sin_addr.s_addr = inet_addr(IP);
	socketInfos.sin_port = htons(_listenPort);
	error = bind(_listenSocket, (SOCKADDR*)&socketInfos, sizeof(socketInfos));
	if (error == SOCKET_ERROR) {
		closesocket(_listenSocket);
		WSACleanup();
		Error("bind", WSAGetLastError());
		Stop();
		return 1;
	}
	error = listen(_listenSocket, _backlog);
	if (error == SOCKET_ERROR) {
		closesocket(_listenSocket);
		WSACleanup();
		Error("listen", WSAGetLastError());
		Stop();
		return 1;
	}
	SOCKET socket = 0;
	while (_continue) {
		socket = accept(_listenSocket, 0, 0);
		if (socket == INVALID_SOCKET) {
			closesocket(_listenSocket);
			WSACleanup();
			Error("accept", WSAGetLastError());
			Stop();
			return 1;
		}
	}closesocket(_listenSocket);
	WSACleanup();
	Accept(socket);
	Stop();
	return 0;
}

DWORD Listener::ThreadLauncher(LPVOID routineParams) {
	return ((Listener*)routineParams)->ListenRoutine();
}