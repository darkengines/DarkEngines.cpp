//////////////////////////////////////////////
//	Filename: Client.cpp
//////////////////////////////////////////////

#include "Client.h"

Client::Client(SOCKET socket) {
	_clientSocket = socket;
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
		part += send(_clientSocket, (const char*)bytes, BUFFER_SIZE, 0);
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
		part += recv(_clientSocket, (char*)bytes, BUFFER_SIZE, 0);
		if (!part) {
			return 0;
		}
		read += part;
	}
	return read;
}
int Client::SendInt(int value) {
	return SendBytes(&value, sizeof(int));
}
int Client::ReceiveInt(int* value) {
	return ReceiveBytes(value);
}
template<typename T>
int Client::Send(T value) {
	return SendBytes(&value, sizeof(T));
}
template<typename T>
int Client::Receive(T* value) {
	return ReceiveBytes(value, sizeof(T));
}