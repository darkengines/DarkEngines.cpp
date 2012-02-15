#include <Listener.h>
#include <Client.h>
#include "THashTable.h"

class Server {
private:
	THashTable<int, Listener*>* _listeners;
public:
	Server();
	int Init();
	void StartListen(char* address, int port);
	void StopListen(char* address, int port);
	void Shutdown();
	void AcceptEventHandler(SOCKET socket);
	void ErrorEventHandler(char* function, int errorCode);
	void StartEventHandler();
	void StopEventHandler();
	void ConnectEventHandler(Client* client);
	void AuthentificationEventHandler(Client* client, void* authentificator);
	void IdentificationEventHandler(Client* client, void* identificator);
	void CommandEventHandler(char* command);
	static DWORD CACAROUTINE(RoutineParams* caca);
private:
	void CleanupListeners();
};