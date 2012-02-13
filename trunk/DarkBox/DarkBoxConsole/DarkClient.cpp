#include "DarkClient.h"

DarkClient::DarkClient() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	_client = new Client(sock);
}

int DarkClient::Connect(char* address, int port) {
	_client->Connect("192.168.1.2", 7777);
	return _client->SendString("Caca dans la bouche !");
	_client->Disconnect();
}