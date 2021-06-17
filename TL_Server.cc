#include "TuteLAN_Server.h"

int main(int argc, char **argv)
{
    TuteLAN_Server ts(argv[1], argv[2]);

    ts.init_game();

    return 0;
}
