#include "TuteLAN_Client.h"

int main(int argc, char **argv)
{
    TuteLAN_Client tc(argv[1], argv[2], argv[3]);

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
