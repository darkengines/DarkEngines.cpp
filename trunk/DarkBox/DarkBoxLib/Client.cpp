//////////////////////////////////////////////
//	Filename: Client.cpp
//////////////////////////////////////////////

#include "Client.h"

Client::Client(SOCKET socket) {
	_clientSocket = socket;
}
int Client::Connect(char* address, int port) {
	sockaddr_in socketInfos;
    socketInfos.sin_family = AF_INET;
    socketInfos.sin_addr.s_addr = inet_addr(address);
    socketInfos.sin_port = htons(port);
    int error = connect(_clientSocket, (SOCKADDR*)&socketInfos, sizeof (socketInfos));
    if (error == SOCKET_ERROR) {
        Error("connect", WSAGetLastError());
        error = closesocket(_clientSocket);
        if (error == SOCKET_ERROR) {
            Error("closesocket", WSAGetLastError());
			return 1;
        }
		return 1;
	}
	Connect();
	return 0;
}
int Client::SendSizeHeader(int size) {
	return send(_clientSocket, (const char*)&size, sizeof(int), 0);
}
int Client::ReceiveSizeHeader(int* size) {
	return recv(_clientSocket, (char*)size, sizeof(int), 0);
}
int Client::SendBytes(void* bytes, int size, bool fireProgress) {
	int sent = 0;
	sent = SendSizeHeader(size);
	if (!sent) {
		return 0;
	}
	int part = sent = 0;
	while (sent < size) {
		part += send(_clientSocket, (const char*)bytes, BUFFER_SIZE, 0);
		if (!part) {
			return 0;
		}
		sent += part;
		if (fireProgress) {
			Progress(sent);
		}
	}
	return sent;
}
int Client::ReceiveBytes(void* bytes, bool fireProgress) {
	int read = 0;
	int size = 0;
	read = ReceiveSizeHeader(&size);
	if (!read || !size) {
		return 0;
	}
	if (fireProgress) {
		SizeHeader(size);
	}
	int part = read = 0;
	while (read < size) {
		part += recv(_clientSocket, (char*)bytes, BUFFER_SIZE, 0);
		if (!part) {
			return 0;
		}
		read += part;
		if (fireProgress) {
			Progress(read);
		}
	}
	return read;
}
char* Client::GetAddress() {
	char* result = 
}
template<typename T>
int Client::Send(T* pValue, int count, bool fireProgress) {
	return SendBytes(value, sizeof(T)*count, fireProgress);
}
template<typename T>
int Client::Receive(T* pValue, int count, bool fireProgress) {
	return ReceiveBytes(value, sizeof(T)*count, fireProgress);
}