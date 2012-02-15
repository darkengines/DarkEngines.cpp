//////////////////////////////////////////////
//	Filename: Client.cpp
//////////////////////////////////////////////

#include "Client.h"

Client::Client(SOCKET socket) {
	_clientSocket = socket;
}
Client::~Client() {
	closesocket(_clientSocket);
}
int Client::Connect(char* address, int port) {
	sockaddr_in socketInfos;
    socketInfos.sin_family = AF_INET;
    socketInfos.sin_addr.s_addr = inet_addr(address);
    socketInfos.sin_port = htons(port);
    int error = connect(_clientSocket, (SOCKADDR*)&socketInfos, sizeof (socketInfos));
    if (error == SOCKET_ERROR) {
        error = closesocket(_clientSocket);
        if (error == SOCKET_ERROR) {
			return WSAGetLastError();
        }
		return WSAGetLastError();
	}
	ConnectEvent();
	return 0;
}
int Client::Disconnect() {
	closesocket(_clientSocket);
	return 0;
}
int Client::SendSizeHeader(int size) {
	return send(_clientSocket, (const char*)&size, sizeof(int), 0);
}
int Client::ReceiveSizeHeader(int* size) {
	return recv(_clientSocket, (char*)size, sizeof(int), 0);
}
int Client::SendBytes(void* bytes, int size) {
	int sent = 0;
	sent = SendSizeHeader(size);
	if (!sent) {
		return 0;
	}
	int part = sent = 0;
	while (sent < size) {
		part += send(_clientSocket, (const char*)bytes+sent, size-sent, 0);
		if (!part) {
			return 0;
		}
		sent += part;
	}
	return sent;
}
int Client::ReceiveBytes(void* bytes) {
	int read = 0;
	int size = 0;
	read = ReceiveSizeHeader(&size);
	if (!read || !size) {
		return 0;
	}
	int part = read = 0;
	while (read < size) {
		part += recv(_clientSocket, (char*)bytes +read, size-read, 0);
		if (!part) {
			return 0;
		}
		read += part;
	}
	return read;
}
int Client::GetPort(int* port) {
	sockaddr_in socketInfos;
	int len = sizeof(socketInfos);
	int error = getsockname(_clientSocket, (sockaddr*)&socketInfos, &len);
	if (error == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	*port = ntohs(socketInfos.sin_port);
	return 0;
}
int Client::GetAddress(char* address) {
	sockaddr_in socketInfos;
	int len = sizeof(socketInfos);
	int error = getsockname(_clientSocket, (sockaddr*)&socketInfos, &len);
	if (error == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	int length = 0;
	GetAddressStringLength(&length);
	memset(address, '\0', length);
	sprintf(address, "%d.%d.%d.%d", socketInfos.sin_addr.S_un.S_un_b.s_b1,
									socketInfos.sin_addr.S_un.S_un_b.s_b2,
									socketInfos.sin_addr.S_un.S_un_b.s_b3,
									socketInfos.sin_addr.S_un.S_un_b.s_b4);
	return 0;
}
int Client::GetAddressStringLength(int* length) {
	sockaddr_in socketInfos;
	int len = sizeof(socketInfos);
	int error = getsockname(_clientSocket, (sockaddr*)&socketInfos, &len);
	if (error == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	in_addr addr = socketInfos.sin_addr;
	*length =  GetByteStringLength(addr.S_un.S_un_b.s_b1)
			 + GetByteStringLength(addr.S_un.S_un_b.s_b2)
			 + GetByteStringLength(addr.S_un.S_un_b.s_b3)
			 + GetByteStringLength(addr.S_un.S_un_b.s_b4)
			 + 4;
	return 0;
}
int Client::GetByteStringLength(unsigned char byte) {
	if (byte > (unsigned char)99) return 3;
	if (byte > (unsigned char)9) return 2;
	return 1;
}
int Client::LaunchRoutine(DWORD(*Routine)(RoutineParams*), RoutineParams* routineParams) {
	routineParams->owner = this;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Routine, (LPVOID)routineParams, 0, 0);
	return 0;
}
int Client::SendString(char* string) {
	return Send<char>(string, strlen(string));
}
int Client::ReceiveString(char** string) {
	int size = 0;
	int read = 0;
	int part = 0;
	read = ReceiveSizeHeader(&size);
	if (read <= 0) {
		return 1;
	}
	*string = (char*)malloc(size+1);
	memset(*string, '\0', size+1);
	read = 0;
	while (read < size) {
		part = recv(_clientSocket, (*string) + read, size-read, 0);
		if (part <= 0) {
			return 1;
		}
		read += part;
	}
	return 0;
}