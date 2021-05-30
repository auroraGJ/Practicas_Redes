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

    int id = connect(sock,result->ai_addr,result->ai_addrlen);

 
    bool envios = true;
    char mensaje[100];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    std::cin >> mensaje;
    while (mensaje[0] != 'Q' && mensaje[1] != '\0')
    {
        
        send(id, mensaje, 100-1, 0);
        ssize_t bytes = recv(id, (void*)mensaje, 100 - 1, 0);
        std::cout  << mensaje << std::endl;
        std::cin >> mensaje;

    }
    close(sock);
    freeaddrinfo(result);

    return 0;
}