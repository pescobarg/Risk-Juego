using namespace std;
#include "continente.h"
#include "jugador.h"
#include "tarjeta.h"
#include <list>
#include <string>
#include <queue>
#include <stack>

#ifndef TABLERO_H
#define TABLERO_H

class Tablero {
private:
  list<Continente> continentes_tablero;
  queue<Jugador*> jugadores_tablero;
  stack<Tarjeta*> tarjetas_tablero;

public:
  queue<Jugador*> getJugadoresTablero();
  stack<Tarjeta*> getTarjetas_tablero();
  list<Continente> getContinentes_tablero ();
  void setJugadoresTablero(queue<Jugador*> jugadores );
  void setTarjetasTablero(stack<Tarjeta*> tarjetas);
  void inicializarJuego();
  void inicializarContinentes();
  void inicializarJugadores() ;
  void inicializarTarjetas();
  void imprimirJugadores();
  int tirarDado();
  int combate(string idAtacante, string idDefensor); 
  int calcularCantUnidadesNuevas(Jugador *jugAux);
  int calcularCantUnidadesCartas(Jugador* nomJug);
  void turnoJugador (string nomJug);
  bool verificarJugadorTurno (string nomJug);
  int AsignarUnidadesJugadores(vector<Jugador> jugadores);
  void eliminarTarjetas(vector<Tarjeta *>& tarjetas_jugador, const string &tipo, int cantidad);
  void faseDraft();
  void faseAtacar();
  void faseFortalecer( );
  void imprimirLineaDeLimitacion(int ancho, char simbolo);
  int  numeroTerritorios();
  vector<Territorio> actualizarTropasEnTerritorios(vector<Territorio> v);
};

#endif