using namespace std;
#include <string>
#include <vector>
#ifndef TERRITORIO_H
#define TERRITORIO_H

class Territorio {
private:
  int Id;
  string nombre_territorio;
  string color_territorio;
  string propietario_territorio;
  vector<Territorio> territorios_alrededor;
  int unidadesDeEjercito_territorio;

public:
  Territorio(int i, string n, string c);
  Territorio(int i, string n, string c, vector<Territorio> a);
  int getId();
  string getNombre();
  string getColor();
  string getPropietario();
  void setColor(string color);
  void setNombre(string nombre);
  void setPropietario(string idJug);
  void setId(int idNueva);
  void establecerPropietario(string prop);
  bool verificarExistenciaPropietario();
  void agregarTerritorioAlrededor(Territorio ter);
  vector<Territorio> getTerritorioAlrededor();
  void setTerritorioAlrededor (vector<Territorio> ta);
  int getUnidadesDeEjercito_territorio();
  void setUnidadesDeEjercito_territorio(int cantidad_unidades);
  void eliminarUnidadesTerritorio();
  void agregarUnidadesTerritorioAdicional(int cantidad_adicional);
};

#endif