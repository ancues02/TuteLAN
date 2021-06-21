#include <thread>
#include "TuteLAN_Client.h"

// Crea los clientes
// Los parametros necesarios son la ip, puerto y nick
// Ejemplo: ./tc 127.0.0.1 2000 ruben
int main(int argc, char **argv)
{
	if(argc != 4){
		std::cout << "Wrong Format, Usage...TODO\n";
		return -1;
	}
	// Crea el cliente
    TuteLAN_Client tc(argv[1], argv[2], argv[3]);
	if(tc.connectToServer(argv[1], argv[2]) == -1) {
		std::cout << "Couldn't connect to server\n";
		return -1;
	}
	// Hace un hilo encargado de recibir mensajes
	std::thread tl_thread([&tc](){ tc.recv_thread(); });
	tl_thread.detach();

	// Mandar el mensaje al servidor
	tc.login();

	// Empezar como tal el cliente
    try {
		tc.start();
	} catch (std::string &e) { // catch errors thrown as strings
		cerr << e << endl;
	} catch (const char *e) { // catch errors thrown as char*
		cerr << e << endl;
	} catch (const std::exception &e) { // catch other exceptions
		cerr << e.what();
	} catch (...) {
		cerr << "Caught and exception of unknown type ..";
	}

	return 0;
}
