#include <thread>
#include "TuteLAN_Client.h"

int main(int argc, char **argv)
{
	if(argc != 4){
		std::cout << "Wrong Format, Usage...TODO\n";
		return -1;
	}
    TuteLAN_Client tc(argv[1], argv[2], argv[3]);
	if(tc.connectToServer(argv[1], argv[2]) == -1) {
		std::cout << "Couldn't connect to server\n";
		return -1;
	}
	std::thread tl_thread([&tc](){ tc.recv_thread(); });
	tl_thread.detach();
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
