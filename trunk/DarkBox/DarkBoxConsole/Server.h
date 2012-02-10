#include "Listener.h"

class Server {
private:
	Listener* listener;
public:
	Server();
	void Start();
	void AcceptEventHandler(SOCKET socket);
private:
};