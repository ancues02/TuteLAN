#include <string.h>

#include "Serializable.h"
#include "TL_Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    addrinfo hints;
    addrinfo *res;

    memset((void*) &hints, 0, sizeof(addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(address, port, &hints, &res);
    if(rc != 0){
        std::cerr << "[getaddrinfo]: " << gai_strerror(rc) << '\n';
        return;
    }

    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    sd = socket(res->ai_family, res->ai_socktype, 0);
    if(sd < 0){
        std::cerr << "[socket]: socket creation\n";
        return;
    }
    sa = *res->ai_addr;
    sa_len = res->ai_addrlen;

    freeaddrinfo(res);
}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recv(sd, buffer, MAX_MESSAGE_SIZE, 0);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);
    
    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    //Serializar el objeto
    obj.to_bin();

    //Enviar el objeto binario a sock usando el socket sd
    if(::send(sd, obj.data(), obj.size(), 0) < 0){
        std::cerr << strerror(errno) << '\n';
        return -1;
    }
    return 0;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    struct sockaddr_in *_sin1 = (struct sockaddr_in*) &s1.sa;
    struct sockaddr_in *_sin2 = (struct sockaddr_in*) &s2.sa;

    return (       _sin1->sin_family == _sin2->sin_family
                && _sin1->sin_addr.s_addr == _sin2->sin_addr.s_addr
                && _sin1->sin_port == _sin2->sin_port   );
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

