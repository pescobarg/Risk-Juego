using namespace std;
#include "territorio.h"
#include <string>
#include <vector>

Territorio::Territorio(int i, string n, string c)
    : Id(i), nombre_territorio(n), color_territorio(c) {}

Territorio::Territorio(int i, string n, string c, vector<Territorio> a)
    : Id(i), nombre_territorio(n), color_territorio(c) , territorios_alrededor(a){}

string Territorio::getNombre() { return nombre_territorio; }

string Territorio::getColor() { return color_territorio; }

int Territorio::getId() { return Id; }

string Territorio::getPropietario() { return propietario_territorio; }

void Territorio::setColor(string color) { color_territorio = color; }

void Territorio:: setPropietario(string idJug){
  propietario_territorio = idJug;
}

void Territorio::setId(int idNueva) { Id = idNueva; }

void Territorio::establecerPropietario(string prop) {
  propietario_territorio = prop;
}

bool Territorio::verificarExistenciaPropietario() {
  if (propietario_territorio.empty()) {
    return false;
  } else {
    return true;
  }
}

void Territorio::agregarTerritorioAlrededor(Territorio ter) {
  territorios_alrededor.push_back(ter);
}

vector<Territorio > Territorio::getTerritorioAlrededor() {
  return territorios_alrededor;
}

int Territorio::getUnidadesDeEjercito_territorio() {
  return unidadesDeEjercito_territorio;
}

void Territorio::setUnidadesDeEjercito_territorio(int cantidad_unidades) {
  unidadesDeEjercito_territorio = cantidad_unidades;
}

void Territorio:: setNombre(string nombre){
  nombre_territorio = nombre;
}

void Territorio::agregarUnidadesTerritorioAdicional(int cantidad_adicional)
{
  unidadesDeEjercito_territorio += cantidad_adicional;
}
void Territorio::eliminarUnidadesTerritorio()
{
  unidadesDeEjercito_territorio -=1;
}
void Territorio::setTerritorioAlrededor (vector<Territorio> ta){
  territorios_alrededor = ta;
}

