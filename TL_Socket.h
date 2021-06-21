#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <ostream>

// -----------------------------------------------------------------------------
// Definiciones adelantadas
// -----------------------------------------------------------------------------
class Socket;
class Serializable;

/**
 *  Esta función compara dos Socks, realizando la comparación de las structuras
 *  sockaddr: familia (INET), dirección y puerto, ver ip(7) para comparar
 *  estructuras sockaddr_in. Deben comparar el tipo (sin_family), dirección
 *  (sin_addr.s_addr) y puerto (sin_port). La comparación de los campos puede
 *  realizarse con el operador == de los tipos básicos asociados.
 */
bool operator== (const Socket &s1, const Socket &s2);

/**
 *  Imprime la dirección y puerto en número con el formato:"dirección_ip:puerto"
 */
std::ostream& operator<<(std::ostream& os, const Socket& dt);
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


/**
 * Clase base que representa el extremo local de una conexión TCP. Tiene la lógica
 * para inicializar un sockect y la descripción binaria del extremo
 *   - dirección IP
 *   - puerto
 */
class Socket
{
public:
    // Capacidad maxima de un mensaje
    static const int32_t MAX_MESSAGE_SIZE = 32000;

    /**
     *  Construye el socket TCP con la dirección y puerto dados. Esta función
     *  usara getaddrinfo para obtener la representación binaria de dirección y
     *  puerto.
     *
     *  Además abrirá el canal de comunicación con la llamada socket(2).
     *
     *    @param address cadena que representa la dirección o nombre
     *    @param port cadena que representa el puerto o nombre del servicio
     *    @param passive booleano que representa si el socket debe tener el flag de passive
     */
    Socket(const char * address, const char * port, bool passive = true);

    /**
     *  Inicializa un Socket copiando los parámetros del socket
     */
    Socket( int sd_,struct sockaddr * _sa, socklen_t _sa_len):sd(sd_), sa(*_sa),
        sa_len(_sa_len){};

    virtual ~Socket(){};

    /**
     *  Recibe un mensaje de aplicación
     *
     *    @param obj que recibirá los datos de la red. Se usará para la
     *    reconstrucción del objeto mediante Serializable::from_bin del interfaz.
     *
     *    @return 0 en caso de éxito o -1 si error (cerrar conexión)
     */
    int recv(Serializable &obj);

    /**
     *  Envía un mensaje de aplicación definido por un objeto Serializable.
     *
     *    @param obj en el que se enviará por la red. La función lo serializará
     *
     *    @return 0 en caso de éxito o -1 si error
     */
    int send(Serializable& obj);

    /**
     *  Enlaza el descriptor del socket a la dirección y puerto
     */
    int bind()
    {
        return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
    }

    /**
     *  Escucha para posibles conexiones al socket
     */
    int listen(int maxClients)
    {
        return ::listen(sd, maxClients);
    }

    /**
     * Acepta la conexion del socket
     */
    int accept( sockaddr *sa_, socklen_t* sa_len_)
    {
        return ::accept(sd, sa_, sa_len_);
    }

    /**
     * Inicializa la conexion del socket
     */
    int connect(const sockaddr *sa_,socklen_t sa_len_ )
    {
        return ::connect(sd,  sa_, sa_len_);
    }

    /**
     * Acaba la conexion que tiene el socket
     */
    int close()
    {
        return ::close(sd);
    }

    friend std::ostream& operator<<(std::ostream& os, const Socket& dt);

    friend bool operator== (const Socket &s1, const Socket &s2);

    int getSockDesc() const { return sd; }
    in_addr_t getAddr() const {return ((struct sockaddr_in*) &sa)->sin_addr.s_addr;}
    in_port_t getPort() const {return ((struct sockaddr_in*) &sa)->sin_port;}
protected:

    /**
     *  Descriptor del socket
     */
    int sd;

    /**
     *  Representación binaria del extremo, usada por servidor y cliente
     */
    struct sockaddr sa;
    socklen_t       sa_len;
};

#endif /* SOCKET_H_ */
