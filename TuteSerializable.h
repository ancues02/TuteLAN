#include <string>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <vector>
#include <iostream>

#include "Serializable.h"
// Clase para serializar los diferentes mensajes y mandar la informacion necesaria


// Tipos de mensaje
enum TuteType : uint8_t { 
    LOGIN,
    ILEGAL_MOVE,
    TURN,
    PINTA,
    CANTE,
    CANTE_TUTE,
    GAME_WINNER,
    TUTE_WINNER,
    CARD,
    HAND,
    DISCONNECT,
    WAIT
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
 *  HAND/CARD   info_1 = number
 *              info_2 = suit
 * 
 * GAME_WINNER  info_1 = equipo ganador del juego
 *              info_2 = puntos obtenidos
 * 
 * TUTE_WINNER  info_1 = equipo ganador de la partida
 * 
 * DISCONNECT   info_1 = id del cliente
 * 
 * WAIT/ILEGAL_MOVE sin info      
*/

class TuteMSG;
bool operator== (const TuteMSG &s1, const TuteMSG &s2);

class TuteMSG: public  Serializable{
public:  
    // Capacidad del mensaje = nick + type + info_1 + info_2
    static const size_t MSG_SIZE = 8 * sizeof(char) + 3 * sizeof(uint8_t); 

    TuteMSG();
    TuteMSG(const std::string& _nick, uint8_t _type, uint8_t _i1, uint8_t _i2);
    virtual ~TuteMSG(){}

    // Serializar los atributos
    void to_bin() override;

    // Deserializar y guardar los valores en los atributos
    int from_bin(char * bobj) override;

    const std::string& getNick() const { return nick; }
    uint8_t getType() const { return type;}
    uint8_t getInfo_1() const { return info_1; }
    uint8_t getInfo_2() const { return info_2; }

    // Compara si dos mensajes son iguales
    friend bool operator== (const TuteMSG &s1, const TuteMSG &s2);

private:
    // Nombre del cliente
    std::string nick;
    // Tipo de mensaje
    uint8_t type;
    // Datos de cada mensaje
    uint8_t info_1;
    uint8_t info_2;
};

