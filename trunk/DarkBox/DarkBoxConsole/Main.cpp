#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

int main(int argc, char** argv) {
	Server* server = new Server();
	server->Start();
	while (1) {

	}
	return 0;
}

