/*
 * Hacer serializable
 */

//#include "Texture.h"
#include <stdlib.h>
#include <unistd.h>

class Card{
public:
    Card(){}
    virtual ~Card(){}
private:
    uint8_t suit;
    uint8_t number; 
};