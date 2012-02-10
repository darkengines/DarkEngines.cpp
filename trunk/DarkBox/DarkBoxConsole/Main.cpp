#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

int main(int argc, char** argv) {
	Server* server = new Server();
	server->Start();
	Sleep(3000);
	server->Stop();
	getchar();
	return 0;
}

