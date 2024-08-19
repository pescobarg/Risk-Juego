#include "vertice.h"

vertice::vertice(Territorio ter) : territorio(ter) {}

void vertice::setVisitado(bool visitado)
{
    this->visitado = visitado;
}
bool vertice::getVisitado()
{
    return this->visitado;
}
Territorio vertice::getTerritorio()
{
    return this->territorio;
}