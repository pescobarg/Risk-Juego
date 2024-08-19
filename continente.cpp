using namespace std;
#include "continente.h"
#include <string>
#include <vector>

Continente::Continente(int i, string n, string c)
    : id(i), nombre_continente(n), color_continente(c) {}

string Continente::getNombre() { return nombre_continente; }

string Continente::getColor() { return color_continente; }

int Continente::getId() {return id; }

vector<Territorio> Continente::getTerritoriosContinente() {
  return territorios_contiente;
}

void Continente::agregarTerritorios(Territorio t) {
  territorios_contiente.push_back(t);
}