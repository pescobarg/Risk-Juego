#ifndef LECTORARCHIVO_H
#define LECTORARCHIVO_H

#include "ArbolHuffman.h"
#include "tablero.h" 
#include <map>
#include <queue>
#include <unordered_map>


using namespace std;

class LectorArchivo {
     
public:
     static Tablero leerArchivoTXT( Tablero tablero, string nombreArchivo, bool &inicializado);
     static Tablero leerArchivoBin(Tablero tablero, string nombreArchivo, bool &inicializado);
};

#endif