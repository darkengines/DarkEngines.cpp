#include "Server.h"
#include <stdio.h>

Server::Server() {
	int i = 0;
	while (i<60000) {
		listener = new Listener("192.168.1.3", i);
		__hook(&Listener::Accept, listener, &Server::AcceptEventHandler);
		__hook(&Listener::Error, listener, &Server::ErrorEventHandler);
		__hook(&Listener::Start, listener, &Server::StartEventHandler);
		__hook(&Listener::Stop, listener, &Server::StopEventHandler);
		listener->LaunchListenRoutine();
		++i;
	}
}

void Server::Start() {
	listener->LaunchListenRoutine();
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