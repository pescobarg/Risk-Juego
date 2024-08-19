//REALIZADO POR SERGIO ASENCIO Y PABLO JAVIER ESCOBAR GOMEZ
using namespace std;
#include <string>
#include <vector>
#include "jugador.h"
#ifndef COMANDOS_H
#define COMANDOS_H

struct Comando {
  string nombre;
  string descripcion;
};

extern const std::vector<Comando> comandos;

void ayuda(string comando_especifico ="");

bool ComandoInicializar(string argumento);

bool ComandoGuardar(string argumento);

bool validarComandoTurno(string argumento);

bool ComandoGuardarComprimido(string argumento);

bool ComandoCostoConquista(string argumento);

bool inicializarSinArgumento();

void inicializarConArgumento(const string &argumento);

#endif