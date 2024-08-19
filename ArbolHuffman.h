#ifndef __ARBOLHUFFMAN_H__
#define __ARBOLHUFFMAN_H__

using namespace std;
#include <map>
#include <string>
#include <queue>

struct NodoHuffman
{
  char caracter;
  long frecuencia;
  NodoHuffman *izquierda;
  NodoHuffman *derecha;
};

struct compararNodos
{
  bool operator()(NodoHuffman *a, NodoHuffman *b)
  {
    return a->frecuencia > b->frecuencia;
  }
};

NodoHuffman *obtenerNodo(char c, long freq, NodoHuffman *izq, NodoHuffman *der);
map<char, string> construirArbolHuffman(string texto, NodoHuffman *&raiz);
map<char, string> construirArbolHuffmanConSecuenciasDeBits(NodoHuffman *&raiz, map<char, long> &frecuencias);
void hallarFrecuencias(string &text, map<char, long> &freq);
void codificarCodigosHuffman(NodoHuffman *raiz, string codigo, map<char, string> &codigos);
void codificarCodigosHuffmanConSecuenciasDeBits(NodoHuffman *raiz, string codigo, map<char, string> &codigos);
void decodificarCodigosHuffman(NodoHuffman *raiz, string linea, int &idx, string& textDecod);
string ajustarTextoCodificado(string textoCodificado, NodoHuffman *raiz, map<char, string> codigosHuff);
int findDepth(NodoHuffman *nodo, char x);
#endif