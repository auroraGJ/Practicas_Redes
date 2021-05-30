#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
         //tamano de la clase jugador 
        _size = sizeof(char) * 80 + sizeof(int16_t) * 2 ;
        //Reservamos memoria
        alloc_data(_size);

        char *dir = _data;
       
        memcpy(dir, &x, sizeof(int16_t));
        dir += sizeof(int16_t);

        memcpy(dir, &y, sizeof(int16_t));
         dir += sizeof(int16_t);

        memcpy(dir, name, 80 * sizeof(char));
        //dir += 80 * sizeof(char);


    }

    int from_bin(char * data)
    {
  
        char *dir = data;
        

        memcpy(&x, dir, sizeof(int16_t));
        dir += sizeof(int16_t);
       
        memcpy(&y, dir, sizeof(int16_t));
        dir += sizeof(int16_t);

        memcpy(name, dir, 80 * sizeof(char));
       // dir += 80 * sizeof(char);
        
        return 0;
    }

    int16_t getX() const{return x;}
    int16_t getY() const{return y;}


public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // 1. Serializar el objeto one_w
     one_w.to_bin();

    // 2. Escribir la serialización en un fichero

     int fichero = open("./player1_data.txt", O_CREAT | O_TRUNC |O_RDWR, 0666);
    ssize_t tamano = write(fichero,one_w.data(), one_w.size());
    if(tamano != one_w.size()) std::cout << "La informacion no se guardo de forma correcta"<< endl;
    close(fichero);

    // 3. Leer el fichero
     fichero = open("./player1_data.txt", O_RDONLY, 0666);
    char buffer[tamano];
    
    if(read(fichero,&buffer,tamano)==-1){
        std::cerr << "Fallo al leer el archivo: ./player1_data.txt\n";
        return -(EXIT_FAILURE);
    } 
    close(fichero);

    // 4. "Deserializar" en one_r
     one_r.from_bin(buffer);

    // 5. Mostrar el contenido de one_r
    std::cout << "Nombre de one_r: " << one_r.name << ", x: " << one_r.getX() << ", y: " << one_r.getY() << "\n";

    return 0;
}

