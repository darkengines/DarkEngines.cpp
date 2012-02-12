#include "Server.h"
#include <stdio.h>

Server::Server() {
	_listeners = new THashTable<int, Listener*>(256, 0);
}

int Server::Init() {
	WORD wVersionRequested;
    WSADATA wsaData;
    int error;
    wVersionRequested = MAKEWORD(2, 2);
	error = WSAStartup(wVersionRequested, &wsaData);
	if (error) {
		return 1;
		WSACleanup();
	}
	return 0;
}

void Server::Shutdown() {
	CleanupListeners();
	delete _listeners;
	WSACleanup();
}

void Server::CleanupListeners() {
	Listener* listener;
	listener = _listeners->GetFirst(0);
	while (listener) {
		if (listener->IsListening()) {
			listener->StopListenRoutine();
		}
		delete listener;
		listener = _listeners->GetNext(0);
	}
}

void Server::StartListen(char* address, int port) {
	Listener* listener = 0;
	listener = new Listener(address, port);
	__hook(&Listener::Accept, listener, &Server::AcceptEventHandler);
	__hook(&Listener::Error, listener, &Server::ErrorEventHandler);
	__hook(&Listener::Start, listener, &Server::StartEventHandler);
	__hook(&Listener::Stop, listener, &Server::StopEventHandler);
	listener->LaunchListenRoutine();
	_listeners->Insert(port, listener);
}

void Server::StopListen(char* address, int port) {
	Listener* listener = _listeners->Find(port);
	listener->StopListenRoutine();
}

void Server::AcceptEventHandler(SOCKET socket) {
	printf("New connection !\n");
	Client* client = new Client(socket);
}
void Server::ErrorEventHandler(char* function, int errorCode) {
	printf("Error %d on function %s.\n", errorCode, function);
}
void Server::StartEventHandler() {
	printf("Start listening.\n");
}
void Server::StopEventHandler() {
	printf("Stop listening.\n");
}
void Server::ConnectEventHandler(Client* client) {

}
void Server::AuthentificationEventHandler(Client* client, void* authentification) {

}
void Server::IdentificationEventHandler(Client* client, void* identification) {

}