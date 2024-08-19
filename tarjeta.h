using namespace std;
#ifndef TARJETA_H
#define TARJETA_H
#include "territorio.h"
#include <string>
#include <list>

class Tarjeta {
  private:
  string tipo;
  Territorio* territorio;
  bool utilizada;
  public:
  Tarjeta(string t, Territorio* te);
  Tarjeta(string t);
  string getTipo();
  Territorio* getTerritorio();
  void setUtilizada(bool v);
  bool getUtilizada();
  void setTipo(string tipoN);
  void setTerritorio(Territorio* territorioN);

};

#endif