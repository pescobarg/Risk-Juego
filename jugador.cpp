using namespace std;
#include "jugador.h"
#include <iostream>
#include <string>
#include <vector>

Jugador::Jugador(string n, string c, string id)
    : nombre_jugador(n),color_jugador(c), id_jugador(id) {}

string Jugador::getNombre() { return nombre_jugador; }

string Jugador::getColor() { return color_jugador; }

string Jugador::getId() { return id_jugador; }

void Jugador::setId(string id)
{
  id_jugador = id;
}

void Jugador::setNombre(string nombre)
{
  nombre_jugador = nombre;
}

void Jugador::setColor(string color)
{
  color_jugador = color;
}

int Jugador::getNumeroTerritorios() { return numeroTerritorios; }

void Jugador::setNumeroTerritorios(int numT)
{
  numeroTerritorios = numT;
}
void Jugador::setTerritorios_jugador(vector<Territorio*> v){
  territorios_jugador = v;
}


int Jugador::getTarjetasCambiadas() { return tarjetasCambiadas; }

void Jugador::setTarjetasCambiadas(int numTar)
{
  tarjetasCambiadas = numTar;
}

int Jugador::getUnidadesDeEjercito() { return unidadesDeEjercito_jugador; }

void Jugador::setUnidadesDeEjercito(int u) { unidadesDeEjercito_jugador = u; }

void Jugador::agregarTarjeta(Tarjeta *t) { tarjetas_jugador.push_back(t); }

vector<Tarjeta *> &Jugador::getTarjetas() { return tarjetas_jugador; }

void Jugador::agregarTerritorio(Territorio *t)
{
  territorios_jugador.push_back(t);
}

void Jugador::eliminarTerritorio(int id)
{
  vector<Territorio *>::iterator it = this->territorios_jugador.begin();
  while (it != this->territorios_jugador.end())
  {
    if (id == (*it)->getId())
    {
 
      it = this->territorios_jugador.erase(it); // Mueve el iterador al siguiente elemento válido después de la eliminación.
    }
    else
    {
      ++it; // Avanza al siguiente elemento si no se elimina.
    }
  }
}


vector<Territorio *> Jugador::getTerritorios_jugador()
{
  return territorios_jugador;
}
void Jugador::agregarUnidadesAdicionales(int cant_adicional)
{
  unidadesDeEjercito_jugador += cant_adicional;
}
void Jugador::eliminarUnidadesEjercito()
{
  unidadesDeEjercito_jugador -= 1;
}
