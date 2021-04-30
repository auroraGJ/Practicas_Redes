
#include <netdb.h>
#include <string.h>
#include <iostream>


 int main(int argc, char **argv)
       {
           struct addrinfo hints;
           struct addrinfo* result;

           memset(&hints, 0, sizeof(struct addrinfo));
            //filtros
           hints.ai_family = AF_UNSPEC;    
           //hints.ai_socktype = SOCK_STREAM; 
      

         int  rcv = getaddrinfo(argv[1], argv[2], &hints, &result);
           if (rcv != 0) {
               //imprimimos el error 
               std::cerr << "[getaddrinfo]: "<<gai_strerror(rcv) << std::endl; 
               return -1;
           }

            //recorremos todos los resultados
           for (auto i = result; i != nullptr; i = i->ai_next) {

               char host[NI_MAXHOST];
               char serv[NI_MAXSERV];
            // cogemos la informacion de la lista con los resultados
               getnameinfo(i->ai_addr,i->ai_addrlen,host,NI_MAXHOST,serv
               ,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);
               //std::cout << "Host :"<< host <<" Port :" << serv << std::endl;
               std::cout  << host << " "<< i->ai_family <<" " <<i->ai_socktype << std::endl;

           } 
           freeaddrinfo(result);           

          return 0; 
    }