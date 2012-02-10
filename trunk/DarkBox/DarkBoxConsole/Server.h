#include "Listener.h"

class Server {
private:
	Listener* listener;
public:
	Server();
	void Start();
	void AcceptEventHandler(SOCKET socket);
	void ErrorEventHandler(char* function, int errorCode);
	void StartEventHandler();
	void StopEventHandler();
private:
};