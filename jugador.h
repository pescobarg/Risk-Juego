using namespace std;
#include <list>
#include "tarjeta.h"
#include <string>
#ifndef JUGADOR_H
#define JUGADOR_H

class Jugador {
private:
  string nombre_jugador;
  string id_jugador;
  string color_jugador;
  int unidadesDeEjercito_jugador;
  int numeroTerritorios;
  int tarjetasCambiadas = 0;
  vector<Tarjeta*> tarjetas_jugador;
  vector<Territorio*> territorios_jugador; 
public:
  Jugador(string n, string c, string id);
  string getNombre();
  string getColor();
  string getId();
  void setId (string id);
  void setNombre (string nombre);
  void setColor (string color);
  int getUnidadesDeEjercito();
  void setUnidadesDeEjercito(int u);
  void setNumeroTerritorios(int numT);
  int getNumeroTerritorios();
  void setTarjetasCambiadas(int numTar);
  int getTarjetasCambiadas();
  void agregarTarjeta (Tarjeta* t);
  vector<Tarjeta*> & getTarjetas ();
  void agregarTerritorio(Territorio* t);
  void eliminarTerritorio(int id);
  vector<Territorio*> getTerritorios_jugador(); 
  void setTerritorios_jugador(vector<Territorio*> v);
  void agregarUnidadesAdicionales(int cant_adicional);
  void eliminarUnidadesEjercito();
};

#endif