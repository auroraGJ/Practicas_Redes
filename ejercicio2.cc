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
    hints.ai_socktype = SOCK_DGRAM; //UDP

    int rcv = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (rcv != 0)
    {
        //imprimimos el error
        std::cerr << "[getaddrinfo]: " << gai_strerror(rcv) << std::endl;
        return -1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

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

    time_t mytime;
    ssize_t tamFecha;
    bool envios = true;
    char mensaje[100];
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    while (envios)
    {
        struct sockaddr cliente;
        socklen_t tam = sizeof(struct sockaddr);
        ssize_t bytes = recvfrom(sock, mensaje, 100 - 1, 0, &cliente, &tam);

        if (bytes == -1)
        {
            std::cerr << "Error en el mensaje" << std::endl;
            return -1;
        }
        getnameinfo(&cliente, tam, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << bytes << " bytes de " << host << ":" << serv << std::endl;
          std::cout << mensaje << std::endl;

        switch (mensaje[0])
        {
        case 'q':
            std::cout << "Saliendo..." << std::endl;
            envios = false;
            break;
        case 'd':
            time(&mytime);
            tamFecha = strftime(mensaje, 100 - 1, "%F", localtime(&mytime));
            sendto(sock, mensaje, tamFecha, 0, &cliente, tam);

            break;
        case 't':
            time(&mytime);
            tamFecha = strftime(mensaje, 100 - 1, "%T %p", localtime(&mytime));
            sendto(sock, mensaje, tamFecha, 0, &cliente, tam);
            break;

        default:
            std::cout << "Comando no soportado " << mensaje[0]<< std::endl;
            sendto(sock, "Comando no soportado\n",22, 0, &cliente, tam);
            break;
        }
    }
    close(sock);
    freeaddrinfo(result);

    return 0;
}