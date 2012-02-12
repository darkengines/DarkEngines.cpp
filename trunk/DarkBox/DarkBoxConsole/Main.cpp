#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

int main(int argc, char** argv) {
	Server* server = new Server();
	server->Init();
	server->StartListen("192.168.1.2", 7777);
	server->StartListen("192.168.1.2", 7776);
	getchar();
	server->StopListen("192.168.1.2", 7777);
	server->StopListen("192.168.1.2", 7776);
	server->Shutdown();
	getchar();
	return 0;
}

