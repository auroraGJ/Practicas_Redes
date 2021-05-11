#include <netdb.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    std::cout << argv[3] << std::endl;
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

    //int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int sock = socket(result->ai_family, result->ai_socktype, 0);

    if (sock == -1)
    {
        std::cerr << "Error al hacer el socket" << std::endl;
        return -1;
    }

    char mensaje[100];
    int bytesEnviados = sendto(sock, argv[3], strlen(argv[3]+1), 0, result->ai_addr, result->ai_addrlen);
    if(bytesEnviados == -1){
         std::cerr << "Error al enviar el mensaje" << std::endl;
        return -1;
    }

    ssize_t bytes = recvfrom(sock, mensaje, 100 - 1, 0, result->ai_addr, &result->ai_addrlen);
      std::cout << mensaje << std::endl;
    
    close(sock);
    freeaddrinfo(result);

    return 0;
}