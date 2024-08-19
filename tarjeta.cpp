using namespace std;
#include "tarjeta.h"
#include <iostream>
#include <vector>

Tarjeta::Tarjeta(string t, Territorio* te)
    : tipo(t), territorio(te){}

Tarjeta::Tarjeta(string t)
    : tipo(t) {}

string Tarjeta::getTipo() { return tipo; }

Territorio* Tarjeta::getTerritorio() { return territorio; }

void Tarjeta::setUtilizada(bool v) { utilizada = v;}

bool Tarjeta::getUtilizada() {return utilizada;}

void Tarjeta::setTipo(string tipoN){
    tipo = tipoN;
}
void Tarjeta::setTerritorio(Territorio* territorioN){
    territorio = territorioN;
}