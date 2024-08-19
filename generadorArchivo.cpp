
#include "generadorArchivo.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <map>
#include <bitset>

using namespace std;

void GenerarArchivo::generarArchivoTXT(Tablero &tablero, string &nombreArchivo)
{

    ofstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    queue<Jugador *> copia_jugador = tablero.getJugadoresTablero();

    archivo << copia_jugador.size() << endl; // numero de jugadores

    while (!copia_jugador.empty())
    {

        Jugador *jugador = copia_jugador.front(); // jugador actual

        archivo << jugador->getNombre() << endl;                     // nombre del jugador
        archivo << jugador->getId() << endl;                         // id del jugador
        archivo << jugador->getColor() << endl;                      // color del jugador
        archivo << jugador->getUnidadesDeEjercito() << endl;         // unidades de ejercito del jugador
        archivo << jugador->getTerritorios_jugador().size() << endl; // tamaño del arreglo de territorios propios del jugador
        archivo << jugador->getTarjetasCambiadas() << endl;          // tarjetas cambiadas del jugador

        archivo << jugador->getTarjetas().size() << endl; // tarjetas totales del jugador
        for (Tarjeta *tarjeta : jugador->getTarjetas())
        {                                          // recorremos el vector de tarjetas del juagdor
            archivo << tarjeta->getTipo() << endl; // tipo de la tarjeta
        }

        for (Territorio *territorio : jugador->getTerritorios_jugador())
        {
            archivo << territorio->getId() << "," << territorio->getUnidadesDeEjercito_territorio() << endl;
        }

        copia_jugador.pop();
    }

    archivo.close();
}

void GenerarArchivo::generarArchivoBin(Tablero &tablero, string &nombreArchivo)
{

    ofstream archivo(nombreArchivo, ios::binary);

    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    queue<Jugador *> copia_jugador = tablero.getJugadoresTablero();

    string textoPlano;
    textoPlano = to_string(copia_jugador.size()) + "\n";

    while (!copia_jugador.empty())
    {

        Jugador *jugador = copia_jugador.front(); // jugador actual

        textoPlano += jugador->getNombre() + "," + jugador->getId() + "," + jugador->getColor() + "," + to_string(jugador->getUnidadesDeEjercito()) + "," + to_string(jugador->getTerritorios_jugador().size()) + "," + to_string(jugador->getTarjetasCambiadas()) + "," + to_string(jugador->getTarjetas().size()) + ";";
        for (Tarjeta *tarjeta : jugador->getTarjetas())
        {                                                       // recorremos el vector de tarjetas del juagdor
            textoPlano = textoPlano + tarjeta->getTipo() + ","; // tipo de la tarjeta
        }
        textoPlano += ";";

        for (Territorio *territorio : jugador->getTerritorios_jugador())
        {
            textoPlano = textoPlano + to_string(territorio->getId()) + "-" + to_string(territorio->getUnidadesDeEjercito_territorio()) + ",";
        }

        textoPlano += "\n";

        copia_jugador.pop();
    }

    NodoHuffman *arbolHuffman = nullptr;
    //cout << "Texto en plano: " << textoPlano << endl;
    map<char, long> freq;
    hallarFrecuencias(textoPlano, freq);

    /*
        cout << "Frecuencias huffman antes:" << endl;
        for (const auto &par : freq)
        {
            char caracter = par.first;
            long f = par.second;

            cout << "Caracter: " << caracter << ", Frecuencia: " << f << endl;
        }
    */

    map<char, string> codigoHuffman = construirArbolHuffman(textoPlano, arbolHuffman);

    short numDif = codigoHuffman.size();
    archivo.write(reinterpret_cast<const char *>(&numDif), sizeof(numDif));

    for (auto par : codigoHuffman)
    {

        char caracter = par.first;
        archivo.write(reinterpret_cast<const char *>(&caracter), sizeof(caracter));
        string codigo = par.second;
        long frecuencia = freq[caracter];
        archivo.write(reinterpret_cast<const char *>(&frecuencia), sizeof(frecuencia));
    }

    //cout << "Codigos huffman antes:" << endl;
    for (const auto &par : codigoHuffman)
    {
        char caracter = par.first;
        string codigo = par.second;

        //cout << "Caracter: " << caracter << ", Codigo Huffman: " << codigo << endl;
    }

    long totalCaracteres = 0;
    for (auto par : freq)
    {
        totalCaracteres += par.second;
    }
    archivo.write(reinterpret_cast<const char *>(&totalCaracteres), sizeof(totalCaracteres));

    string str = "";
    string bits_concatenados = "";
    string bits_concatenados_string = "";

    for (char a : textoPlano)
    {
        str += codigoHuffman[a];
    }

    for (char bit : str)
    {
        bits_concatenados += bit;
        if (bits_concatenados.length() == 8)
        {
            bitset<8> bits(bits_concatenados);
            bits_concatenados_string += bits.to_string();
            archivo.write(reinterpret_cast<const char *>(&bits), sizeof(char));

            bits_concatenados = "";
        }
    }

    if (!bits_concatenados.empty())
    {
        bits_concatenados += string(8 - bits_concatenados.length(), '0');
        bitset<8> bits(bits_concatenados);
        bits_concatenados_string += bits.to_string();
        archivo.write(reinterpret_cast<const char *>(&bits), sizeof(char));
    }
/*
    cout << "Mensaje cod1:" << str << endl;

    cout << "Mensaje cod2:" << bits_concatenados_string << endl;

    string textoDecod;

    int index = -1;
    cout << "Mensaje decodificado: \n";

    while (index < ((int)str.size() - 2))
    {
        decodificarCodigosHuffman(arbolHuffman, str, index, textoDecod);
    }
    cout << textoDecod << endl;
*/
    archivo.close();
}
