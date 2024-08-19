#ifndef _GRAFO_H_
#define _GRAFO_H_

#include <iostream>
#include <queue>
#include <vector>
#include <limits>

#include "vertice.h" 
#include "tablero.h"

using namespace std;

class GrafoAdyacencia {
protected:
  static const int ARISTA_NULA = 0.0f;

  vector<vertice *> vertices;
  vector<vector<float>> aristas;

  vector<vector<int>> camino;
  vector<vector<float>> dist;

  vector<bool> visitados;

  int nmbVertices;
  int maxVertices;
  double distanciaTotal = 0;

public:
  GrafoAdyacencia(int tamano);
  ~GrafoAdyacencia();
  void conquista_barata(Tablero tablero);
  void costo_conquista(Tablero tablero, string territorioHacia);
  void Dijkstra(vertice *verticeOrigen, Tablero tab);
  void FloydWarshall(Tablero tab);
  void inicializarFloydWarshall();
  vector<int> construirCamino (int u, int v);
  void imprimirCamino(vector<int>& cam);
  int getNumVertices();
  vertice *getVertices(int indice);
  vector<vertice *> getListaVertices();
  bool es_vacio();
  bool grafo_lleno();
  void imprimir_aristas();
  void agregar_vertice(vertice *auxVer);
  void agregar_arista(vertice* desdeVertice, vertice* haciaVertice);
  void llenar_matriz(Tablero tab);
  int getIndice(vertice *auxVer);
  int getIndicePorID (int id);
  void vaciar_visitados();
  double getDistanciaTotal();
  vertice* encontrarVerticeCorrespondiente( Territorio& territorioBuscado);
  bool verificarConquista(Tablero tab, string terBuscado);
  vertice* buscarVertice (string nombreVer);

};

#endif
