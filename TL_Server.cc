#include "TuteLAN_Server.h"

// Crea el servidor
// Los parametros necesarios son la ip y puerto 
// Ejemplo: ./tc 127.0.0.1 2000

int main(int argc, char **argv)
{
    TuteLAN_Server ts(argv[1], argv[2]);

    ts.init_game();

    return 0;
}
