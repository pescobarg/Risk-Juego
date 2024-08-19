#ifndef GENERADORARCHIVO_H
#define GENERADORCHIVO_H

#include "tablero.h" 
#include "ArbolHuffman.h"


using namespace std;

class GenerarArchivo {
public:
    static void generarArchivoTXT( Tablero& tablero, string& nombreArchivo);
    static void generarArchivoBin( Tablero& tablero, string& nombreArchivo);
};

#endif