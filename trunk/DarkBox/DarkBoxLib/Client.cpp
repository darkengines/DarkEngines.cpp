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
		part += send(_clientSocket, (const char*)bytes+sent, size-sent, 0);
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
		part += recv(_clientSocket, (char*)bytes +read, size-read, 0);
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
	return 0;
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
	return 0;
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

DWORD Client::ThreadLauncher(LPVOID routineParams) {
	RoutineParams* params = (RoutineParams*)routineParams;
	switch (params->RoutineCode) {
		case (0): {
			return params->owner->CommandRoutine();
			break;
		}
		case (1): {
			return params->owner->UploadRoutine();
			break;
		}
		case (2): {
			return params->owner->DownloadRoutine();
			break;
		}
		default: {
			return 0;
		}
	}
}
int Client::LaunchCommandRoutine() {
	_continue = true;
	RoutineParams* params = new RoutineParams();
	params->owner = this;
	params->RoutineCode = 0;
	_clientThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadLauncher, (LPVOID)params, 0, 0);
	if (_clientThread == 0) {
		Error("CreateThread", GetLastError());
		return 1;
	}
	return 0;
}
DWORD Client::CommandRoutine() {
	_continue = true;
	int size = 0;
	int read = 0;
	int part = 0;
	while (_continue) {
		read = ReceiveSizeHeader(&size);
		if (read <= 0) {
			return 1;
		}
		char* cmd = (char*) malloc(size+1);
		memset(cmd, '\0', size+1);
		read = 0;
		while (read < size) {
			part = recv(_clientSocket, cmd + read, size-read, 0);
			if (part <= 0) {
				return 1;
			}
			read += part;
		}
		Command(cmd);
		free(cmd);
	}
	return 0;
}
DWORD Client::UploadRoutine() {
	return 0;
}
DWORD Client::DownloadRoutine() {
	return 0;
}