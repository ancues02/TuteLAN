#include "TuteSerializable.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Constructora con valores por defecto
TuteMSG::TuteMSG(): 
    nick("Default"), 
    type(TuteType::LOGIN), 
    info_1(0), 
    info_2(0) 
{
} 
// Constructora con todos los atributos que necesitamos
TuteMSG::TuteMSG(const std::string& _nick, uint8_t _type, uint8_t _i1, uint8_t _i2): 
    nick(_nick), 
    type(_type), 
    info_1(_i1), 
    info_2(_i2) 
{
} 

void TuteMSG::to_bin()
{

    alloc_data(MSG_SIZE);
    memset(_data, 0, MSG_SIZE);

    char* tmp=_data;

    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &info_1, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &info_2, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, nick.c_str(), 8 * sizeof(char));
    tmp[7*sizeof(char)]='\0';
}

int TuteMSG::from_bin(char * bobj)
{
    char* tmp = bobj;

    memcpy(&type,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&info_1,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&info_2,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    tmp[7 * sizeof(char)] = '\0';
    nick = tmp;

    return 0;
}


bool operator== (const TuteMSG &s1, const TuteMSG &s2){
    return s1.info_1 == s2.info_1 && s1.info_2 == s2.info_2;
}