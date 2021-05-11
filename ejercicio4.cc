#include <netdb.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    //filtros
    hints.ai_family = AF_INET;      //IPv4
    hints.ai_socktype = SOCK_STREAM; //UDP

    int rcv = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (rcv != 0)
    {
        //imprimimos el error
        std::cerr << "[getaddrinfo]: " << gai_strerror(rcv) << std::endl;
        return -1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        std::cerr << "Error al hacer el socket" << std::endl;
        return -1;
    }

    int b = bind(sock, result->ai_addr, result->ai_addrlen);

    if (b == -1)
    {
        std::cerr << "Error al hacer el bind" << std::endl;
        return -1;
    }

    int c = listen(sock,1);
       if (c == -1)
    {
        std::cerr << "Error al hacer el listen" << std::endl;
        return -1;
    }
    struct sockaddr cliente;
    socklen_t tam = sizeof(struct sockaddr);
    int id = accept(sock,&cliente,&tam);
       if (id == -1)
    {
        std::cerr << "Error al hacer el accept" << std::endl;
        return -1;
    }

 
    bool envios = true;
    char mensaje[100];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    getnameinfo(&cliente, tam, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    std::cout  << "Conexion desde " << host << ":" << serv << std::endl;
    while (envios)
    {
        ssize_t bytes = recv(id, (void*)mensaje, 100 - 1, 0);

        if (bytes == -1)
        {
            std::cerr << "Error en el mensaje" << std::endl;
            return -1;
        }
        send(id, mensaje, bytes, 0);
    }
    close(sock);
    freeaddrinfo(result);

    return 0;
}