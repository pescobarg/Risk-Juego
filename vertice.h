#include <iostream>
#include "territorio.h"

class vertice
{
  protected:
    Territorio territorio;
    bool visitado = false;
  public:
    vertice(Territorio ter);
    void setVisitado(bool visitado);
    Territorio getTerritorio();
    bool getVisitado();

};
