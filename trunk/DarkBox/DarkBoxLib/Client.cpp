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
	Connect(this);
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
int Client::GetPort(int* port) {
	sockaddr_in socketInfos;
	int len = sizeof(socketInfos);
	int error = getsockname(_clientSocket, (sockaddr*)&socketInfos, &len);
	if (error == SOCKET_ERROR) {
		Error("getsockname", WSAGetLastError());
		return 1;
	}
	*port = ntohs(socketInfos.sin_port);
}
int Client::GetAddress(char* address) {
	sockaddr_in socketInfos;
	int len = sizeof(socketInfos);
	int error = getsockname(_clientSocket, (sockaddr*)&socketInfos, &len);
	if (error == SOCKET_ERROR) {
		Error("getsockname", WSAGetLastError());
		return 1;
	}
	int length = 0;
	GetAddressStringLength(&length);
	memset(address, '\0', length);
	sprintf(address, "%d.%d.%d.%d", socketInfos.sin_addr.S_un.S_un_b.s_b1,
									socketInfos.sin_addr.S_un.S_un_b.s_b2,
									socketInfos.sin_addr.S_un.S_un_b.s_b3,
									socketInfos.sin_addr.S_un.S_un_b.s_b4);
}
int Client::GetAddressStringLength(int* length) {
	sockaddr_in socketInfos;
	int len = sizeof(socketInfos);
	int error = getsockname(_clientSocket, (sockaddr*)&socketInfos, &len);
	if (error == SOCKET_ERROR) {
		Error("getsockname", WSAGetLastError());
		return 1;
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
template<typename T>
int Client::Send(T* pValue, int count, bool fireProgress) {
	return SendBytes(value, sizeof(T)*count, fireProgress);
}
template<typename T>
int Client::Receive(T* pValue, int count, bool fireProgress) {
	return ReceiveBytes(value, sizeof(T)*count, fireProgress);
}