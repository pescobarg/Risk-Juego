using namespace std;
#include <iostream>
#include <map>
#include <queue>
#include <bitset>
#include <string>
#include "ArbolHuffman.h"

using namespace std;

NodoHuffman *obtenerNodo(char c, long freq, NodoHuffman *izq, NodoHuffman *der)
{
  NodoHuffman *nodo = new NodoHuffman();
  nodo->caracter = c;
  nodo->frecuencia = freq;
  nodo->izquierda = izq;
  nodo->derecha = der;

  return nodo;
}

void hallarFrecuencias(string &text, map<char, long> &freq)
{
  for (char sim : text)
  {
    freq[sim]++;
  }
}

map<char, string> construirArbolHuffman(string texto, NodoHuffman *&raiz)
{
  priority_queue<NodoHuffman *, vector<NodoHuffman *>, compararNodos> cola;

  map<char, long> frecuencias;
  for (char sim : texto)
  {
    frecuencias[sim]++;
  }

  for (auto par : frecuencias)
  {
    cola.push(obtenerNodo(par.first, par.second, nullptr, nullptr));
  }

  while (cola.size() != 1)
  {
    NodoHuffman *izquierda = cola.top();
    cola.pop();
    NodoHuffman *derecha = cola.top();
    cola.pop();
    long sumaFreq = izquierda->frecuencia + derecha->frecuencia;
    NodoHuffman *padre = obtenerNodo('\0', sumaFreq, izquierda, derecha);
    cola.push(padre);
  }

  raiz = cola.top();

  map<char, string> codigoHuffman;
  codificarCodigosHuffman(raiz, "", codigoHuffman);

  return codigoHuffman;
}

map<char, string> construirArbolHuffmanConSecuenciasDeBits(NodoHuffman *&raiz, map<char, long> &frecuencias)
{
  priority_queue<NodoHuffman *, vector<NodoHuffman *>, compararNodos> cola;

  for (auto par : frecuencias)
  {
    cola.push(obtenerNodo(par.first, par.second, nullptr, nullptr));
  }

  while (cola.size() != 1)
  {
    NodoHuffman *izquierda = cola.top();
    cola.pop();
    NodoHuffman *derecha = cola.top();
    cola.pop();
    long sumaFreq = izquierda->frecuencia + derecha->frecuencia;
    NodoHuffman *padre = obtenerNodo('\0', sumaFreq, izquierda, derecha);
    cola.push(padre);
  }

  raiz = cola.top();

  map<char, string> codigoHuffman;
  codificarCodigosHuffman(raiz, "", codigoHuffman);

  return codigoHuffman;
}

void codificarCodigosHuffman(NodoHuffman *raiz, string codigo, map<char, string> &codigos)
{
  if (!raiz)
  {
    return;
  }
  if (!raiz->derecha && !raiz->derecha)
  {
    codigos[raiz->caracter] = codigo;
  }
  codificarCodigosHuffman(raiz->izquierda, codigo + "0", codigos);
  codificarCodigosHuffman(raiz->derecha, codigo + "1", codigos);
}

void codificarCodigosHuffmanConSecuenciasDeBits(NodoHuffman *raiz, string codigo, map<char, string> &codigos)
{
  if (!raiz)
  {
    return;
  }
  if (!raiz->derecha && !raiz->derecha)
  {
    bitset<16> bits(codigo);
    codigos[raiz->caracter] = bits.to_string();
  }
  codificarCodigosHuffmanConSecuenciasDeBits(raiz->izquierda, codigo + "0", codigos);
  codificarCodigosHuffmanConSecuenciasDeBits(raiz->derecha, codigo + "1", codigos);
}

void decodificarCodigosHuffman(NodoHuffman *raiz, string linea, int &idx, string& textDecod)
{
  if (!raiz)
  {
    return;
  }
  if (!raiz->izquierda && !raiz->derecha)
  {
    textDecod += raiz->caracter;
    return;
  }
  idx++;
  if (linea[idx] == '0')
  {
    decodificarCodigosHuffman(raiz->izquierda, linea, idx, textDecod);
  }
  else
  {
    decodificarCodigosHuffman(raiz->derecha, linea, idx, textDecod);
  }
}

// Función que elimina los ceros 0 agregados cuando se escribió el archivo binario. Por ejemplo, al código Huffman 11 se le agregaban
// 6 ceros para que quede en 1 byte completo, es decir, quedaba en 00000011, entonces esta función pretende quitar esos ceros, de lo
// contrario la decodificación falla.
string ajustarTextoCodificado(string textoCodificado, NodoHuffman *raiz, map<char, string> codigosHuff)
{
  string textoAjustado = "";
  if (!raiz)
  {
    return textoCodificado;
  }

  for (int i = 0; i < textoCodificado.length(); i = i + 16)
  {
    string porcion = textoCodificado.substr(i, 16);
    for (auto &it : codigosHuff)
    {
      if (it.second == porcion)
      {
        int profundidad = findDepth(raiz, it.first);
        textoAjustado += textoCodificado.substr(i + 16 - profundidad, profundidad);
      }
    }
  }

  return textoAjustado;
}

int findDepth(NodoHuffman *nodo, char x)
{
  if (nodo == NULL)
    return -1;

  int dist = -1;

  if ((nodo->caracter == x)
      || (dist = findDepth(nodo->izquierda, x)) >= 0
      || (dist = findDepth(nodo->derecha, x)) >= 0)
    return dist + 1;

  return dist;
}
