#include "Server.h"
#include <stdio.h>

Server::Server() {
	listener = new Listener("192.168.1.3", 7777);
	__hook(&Listener::Accept, listener, &Server::AcceptEventHandler);
	__hook(&Listener::Error, listener, &Server::ErrorEventHandler);
	__hook(&Listener::Start, listener, &Server::StartEventHandler);
	__hook(&Listener::Stop, listener, &Server::StopEventHandler);
}

void Server::Start() {
	listener->LaunchListenRoutine();
}

void Server::Stop() {
	listener->StopListenRoutine();
}

void Server::AcceptEventHandler(SOCKET socket) {
	printf("New connection !\n");
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