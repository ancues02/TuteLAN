#include "Card.h"


Card::Card( uint8_t number_, uint8_t suit_):suit(suit_) , number(number_), client_ID(-1), nick("none"){} 

void Card::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp=_data;

    memcpy(tmp, &suit, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &number, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &client_ID, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, nick.c_str(), 8 * sizeof(char));

}

int Card::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    char* tmp = _data;

    memcpy(&suit,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&number,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t); 

    memcpy(&nick,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);  

    tmp[7 * sizeof(char)] = '\0';//capamos el nombre a 8 letras
    nick=tmp;

    return 0;
}
