#include "Server.h"
#include <stdio.h>

Server::Server() {
	listener = new Listener("192.168.1.2", 7777);
	__hook(&Listener::Accept, listener, &Server::AcceptEventHandler);
}

void Server::Start() {
	listener->LaunchListenRoutine();
}

void Server::AcceptEventHandler(SOCKET socket) {
	printf("new connection !\n");
}