//Texture.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <vector>
#include <iostream>

#include "Serializable.h"

enum TuteType : uint8_t { 
    PRUEBA,
    LOGIN,
    ILEGAL_MOVE,
    TURN,
    PINTA,
    CANTE,
    CANTE_TUTE,
    ROUND_WINNER,
    GAME_WINNER,
    CARD,
    HAND
};

/*
 * 
 *  LOGIN / para el server          nick
 *          para el cliente         player ID
 *          
 *  PINTA       info_1 = pinta
 * 
 *  TURN        info_1 = playerTurn
 * 
 *  CANTE       info_1 = player
 *              info_2 = suit
 * 
 *  CANTE_TUTE  info_1 = player
 * 
 *  HAND/CARD:  info_1 = number
 *              info_2 = suit
*/

class TuteMSG;
bool operator== (const TuteMSG &s1, const TuteMSG &s2);

class TuteMSG: public  Serializable{
public:  
    static const size_t MSG_SIZE = 8 * sizeof(char) + 3 * sizeof(uint8_t); //

    TuteMSG();
    TuteMSG(const std::string& _nick, uint8_t _type, uint8_t _i1, uint8_t _i2);
    virtual ~TuteMSG(){}

    void to_bin() override;

    int from_bin(char * bobj) override;

    const std::string& getNick() const { return nick; }
    uint8_t getType() const { return type;}
    uint8_t getInfo_1() const { return info_1; }
    uint8_t getInfo_2() const { return info_2; }

    friend bool operator== (const TuteMSG &s1, const TuteMSG &s2);

private:

    std::string nick;
    uint8_t type;
    uint8_t info_1;
    uint8_t info_2;
};

