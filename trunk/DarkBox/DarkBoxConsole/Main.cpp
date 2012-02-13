#include <stdio.h>
#include <stdlib.h>
#include "Server.h"
#include "DarkClient.h"

int main(int argc, char** argv) {
	Server* server = new Server();
	server->Init();
	server->StartListen("192.168.1.2", 7777);
	server->StartListen("192.168.1.2", 7776);
	DarkClient* client = new DarkClient();
	client->Connect("192.168.1.2", 7777);
	getchar();
	server->StopListen("192.168.1.2", 7777);
	server->StopListen("192.168.1.2", 7776);
	server->Shutdown();
	getchar();
	return 0;
}

