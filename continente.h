using namespace std;
#include "territorio.h"
#include <string>
#include <vector>
#ifndef CONTINENTE_H
#define CONTINENTE_H

class Continente {
private:
  int id;
  string nombre_continente;
  string color_continente;
  vector<Territorio> territorios_contiente;

public:
  Continente(int i, string n, string c);
  string getNombre();
  string getColor();
  int getId();
  vector<Territorio> getTerritoriosContinente();
  void agregarTerritorios(Territorio t);
};
#endif