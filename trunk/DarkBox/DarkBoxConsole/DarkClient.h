#include <Client.h>
#include <Windows.h>

class DarkClient {
private:
	Client* _client;
public:
	DarkClient();
	int Connect(char* address, int port);
	void ConnectEventHandler();
private:
};