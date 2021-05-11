#include <netdb.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <thread>

#define MAX_THREADS 5



class MessageThread
{
private:
    int sock;
    int id;

public:
    MessageThread(int sock_, int i_) : sock(sock_), id(i_) {}

    void hacer_mensaje()
    {
        time_t mytime;
        size_t tam;
        char mensaje[100];
        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];

        struct sockaddr cliente;
        //socklen_t clientLength = sizeof(struct sockaddr);
        socklen_t tam = sizeof(struct sockaddr);

        while (true)
        {
            
            sleep(3); 

            int bytes = recvfrom(sock, (void *)mensaje, (100- 1) * sizeof(char), 0, &cliente, &tam);
            mensaje[100] = '\0';
            
            int i = 0;
            

            if (bytes== -1)
            {
                std::cerr << "Error en la recepcion "<< std::endl;
                return;
            }

            getnameinfo(&cliente, tam, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
            std::cout << bytes << " bytes de " << host << ":" << serv << ". Thread: " << std::this_thread::get_id() << "\n";


            switch (mensaje[0])
            {


            case 't':
                time(&mytime);
                tam = strftime(mensaje, 100 - 1, "%T %p", localtime(&mytime));
                sendto(sock,mensaje, tam, 0, &cliente, tam);
                break;


            case 'd':
                time(&mytime);
                tam = strftime(mensaje, 100 - 1, "%F", localtime(&mytime));
                sendto(sock ,mensaje, tam, 0, &cliente, tam);
                break;


            default:
                std::cout << "Comando no soportado: " << mensaje[0] << "\n";
                sendto(sock, "Comando no soportado\n", 22, 0, &cliente, tam);
            }
        }
    }
};


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

    bool serverActive = true;

    std::thread threads[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++)
    {
        MessageThread *m = new MessageThread(sock, i);
        std::thread([&m]() {m->hacer_mensaje(); delete m; });
    }

    //El thread principal es el que procesa el cierre del servidor cuando
    //se recibe una q
    std::string q;
    do
    {
        std::cin >> q;
    } while (q != "q");



    close(sock);
    freeaddrinfo(result);

    return 0;
}