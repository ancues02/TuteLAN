#include "TuteLAN.h"

#include <iostream>

using namespace std;

void start(int ac, char **av) {
	TuteLAN_Client tl(ac, 2, "hola");	//para que compile
	tl.start();
}

int main(int ac, char **av) {

	try {
		start();
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
