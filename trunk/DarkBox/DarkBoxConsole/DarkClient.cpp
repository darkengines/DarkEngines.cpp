#include "DarkClient.h"

DarkClient::DarkClient() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	_client = new Client(sock);
	__hook(&Client::ConnectEvent, _client, &DarkClient::ConnectEventHandler);
}
int DarkClient::Connect(char* address, int port) {
	_client->Connect(address, port);
	return 0;
}
void DarkClient::ConnectEventHandler() {
	_client->SendString("command");
	int i = 0;
	while (i<10) {
		_client->SendString("My command Yeah !");
		++i;
	}
	_client->Disconnect();
}