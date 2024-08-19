#include "tablero.h"
#include <map>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cstdlib>
#include "time.h"

using namespace std;

void Tablero::inicializarJuego()
{
  inicializarContinentes();
  inicializarTarjetas();
  inicializarJugadores();
}

void Tablero::inicializarJugadores()
{
  int numJugadores;
  bool argumento_correcto = false;
  bool nombre_valido = false;
  bool color_valido = false;
  vector<Jugador *> jug_aux;
  do
  {
    cout << "Ingrese la cantidad de jugadores: ";
    // Pedir al usuario el ingreso de los jugadores
    if (cin >> numJugadores)
    {
      if (numJugadores >= 2 && numJugadores <= 6)
      {
        for (int i = 0; i < numJugadores; i++)
        {
          string nombre, color;
          do
          {
            cout << "Ingrese el nombre del Jugador " << i + 1 << ": ";
            cin >> nombre;
            nombre_valido = true;
            for (Jugador *j : jug_aux)
            {
              if (j->getNombre() == nombre)
              {
                nombre_valido = false;
                cout << "El nombre " << nombre << " ya fue escogido" << endl;
                break;
              }
            }
          } while (!nombre_valido);
          do
          {
            cout << "Ingrese el color del Jugador " << i + 1 << ": ";
            cin >> color;
            color_valido = true;
            for (Jugador *j : jug_aux)
            {
              if (j->getColor() == color)
              {
                color_valido = false;
                cout << "El color " << color << " ya fue escogido" << endl;
              }
            }
          } while (!color_valido);

          Jugador *jugNuevo = new Jugador(nombre, color, to_string(i + 1));
          jug_aux.push_back(jugNuevo);
          jugadores_tablero.push(jugNuevo);
        }
        argumento_correcto = true;
      }
      else
      {
        cout << "Numero de jugadores insuficiente o incorrecto" << endl;
      }
    }
    else
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Entrada invalida. Por favor, ingrese un numero valido." << endl;
    }

  } while (!argumento_correcto);

  // Llenamos un vector auxiliar de territorios disponibles
  vector<Territorio> TerritorioDisponible;
  for (Continente continente : continentes_tablero)
  {
    for (Territorio territorio : continente.getTerritoriosContinente())
    {
      TerritorioDisponible.push_back(territorio);
    }
  }

  bool TodoIngresado = false;
  vector<Jugador> AuxJug;
  queue<Jugador *> jugadores_aux = jugadores_tablero;
  // Extraer elementos de la cola y agregarlos al vector
  while (!jugadores_aux.empty())
  {
    Jugador *jugador_ptr = jugadores_aux.front(); // Obtener el puntero al jugador
    // Desreferenciar el puntero y agregarlo al vector
    if (jugador_ptr != nullptr)
    {
      Jugador jugador = *jugador_ptr;
      AuxJug.push_back(jugador);
      jugadores_aux.pop(); // Quitar el jugador de la cola
    }
  }

  do
  {
    for (Jugador &jugador : AuxJug)
    {
      int idTerritorio;
      // Imprimir todos los territorios disponibles
      cout << "Territorios disponibles:" << endl;
      for (Territorio territorio : TerritorioDisponible)
      {
        cout << territorio.getId() << " " << territorio.getNombre() << endl;
      }
      bool IdCorrecta = false;

      do
      {
        cout << "Ingrese el territorio para " << jugador.getNombre() << ": ";
        cin >> idTerritorio;

        for (size_t i = 0; i < TerritorioDisponible.size(); ++i)
        {
          if (TerritorioDisponible[i].getId() == idTerritorio)
          {
            Territorio *territorioAux = new Territorio(TerritorioDisponible[i].getId(), TerritorioDisponible[i].getNombre(), TerritorioDisponible[i].getColor(), TerritorioDisponible[i].getTerritorioAlrededor());
            territorioAux->setPropietario(jugador.getId());
            jugador.agregarTerritorio(territorioAux);
            TerritorioDisponible.erase(TerritorioDisponible.begin() + i);
            IdCorrecta = true;
            break;
          }
        }
        if (!IdCorrecta)
        {
          cout << "La ID " << idTerritorio << " no corresponde a ningun id disponible o asignable." << endl;
        }
      } while (!IdCorrecta);

      if (TerritorioDisponible.size() == 0)
      {

        TodoIngresado = true;
        break;
      }
    }
  } while (!TodoIngresado);

  int unidadesIniciales = AsignarUnidadesJugadores(AuxJug);
  for (Jugador &jugador : AuxJug)
  {
    jugador.setUnidadesDeEjercito(unidadesIniciales);
  }

  // Se guarda la cantidad de unidades nuevas disponibles dentro del map con el id del jugador restando 
  // las unidades iniciales que se coloca en cada uno de los territorios de los jugadores
  map<string, int> unidadesNuevasJugadores;
  for (Jugador &jugador : AuxJug)
  {
    int nuevasUnidades = jugador.getUnidadesDeEjercito();
    int cont = 0;

    for (Territorio *territorio : jugador.getTerritorios_jugador())
    {
      territorio->setUnidadesDeEjercito_territorio(1);
      cont++;
    }
    nuevasUnidades -= cont;
    unidadesNuevasJugadores[jugador.getId()] = nuevasUnidades;
  }
  
  cout << endl;
  imprimirLineaDeLimitacion(40, '_');
  cout << "ASIGNACION DE TROPAS" << endl;
  bool totalidadUnidadesIngresadas = false;
  do
  {
    int contJugadoresFinalizados = 0;
    for (Jugador &jugador : AuxJug)
    {
      bool territorioCorrecto = false;
      int entradaTerritorio;
      int nuevasUnidades = unidadesNuevasJugadores[jugador.getId()];
      if (nuevasUnidades > 0)
      {
        cout << endl;
        cout << "Jugador: " << jugador.getNombre() << endl;
        for (Territorio *territorio : jugador.getTerritorios_jugador())
        {
          cout << territorio->getId() << ". " << territorio->getNombre() << ": " << territorio->getUnidadesDeEjercito_territorio() << endl;
        }

        do
        {
          cout << "Usted cuenta con " << nuevasUnidades << " unidades para asignar." << endl;
          cout << "A cual territorio quiere asignarlas?: ";
          cin >> entradaTerritorio;

          for (Territorio *territorio : jugador.getTerritorios_jugador())
          {
            if (territorio->getId() == entradaTerritorio)
            {
              territorioCorrecto = true;
              bool cantidadUnidades = false;
              int unidadesIngresadas = 0;
              do
              {
                cout << "Cuantas unidades quiere asignar?: ";
                cin >> unidadesIngresadas;
                if (unidadesIngresadas > 0 && unidadesIngresadas <= nuevasUnidades)
                {
                  cantidadUnidades = true;
                  territorio->agregarUnidadesTerritorioAdicional(unidadesIngresadas);
                  nuevasUnidades -= unidadesIngresadas;
                  unidadesNuevasJugadores[jugador.getId()] = nuevasUnidades;
                }
                else
                {
                  cout << "Cantidad de unidades invalida" << endl;
                }
              } while (!cantidadUnidades);
            }
          }
          if (!territorioCorrecto)
          {
            cout << "El ID " << entradaTerritorio << " no es valido." << endl;
          }
        } while (!territorioCorrecto);
      }
      else
      {
        contJugadoresFinalizados++;
      }
    }

    if (contJugadoresFinalizados == AuxJug.size())
    {
      totalidadUnidadesIngresadas = true;
    }

  } while (!totalidadUnidadesIngresadas);

  for (Jugador &jugador : AuxJug)
  {
    jugador.setNumeroTerritorios(jugador.getTerritorios_jugador().size()); // Añado la cantidad de territorios conquistados de cada jugador a una variable
    cout << endl
         << endl;
    cout << jugador.getNombre() << endl;
    for (Territorio *territorio : jugador.getTerritorios_jugador())
    {
      cout << "Territorio: " << territorio->getNombre() << " - Unidades: " << territorio->getUnidadesDeEjercito_territorio() << endl;
    }
  }

  // Vaciar la cola original (si es necesario)
  while (!jugadores_tablero.empty())
  {
    delete jugadores_tablero.front();
    jugadores_tablero.pop();
  }

  // Copiar elementos de AuxJug de vuelta a la cola original
  for (const Jugador &jugador : AuxJug)
  {
    Jugador *jugador_ptr = new Jugador(jugador); // Crear una nueva copia del jugador
    jugadores_tablero.push(jugador_ptr);         // Agregar la copia a la cola original
  }
}

void Tablero::imprimirJugadores()
{
  queue<Jugador *> q = jugadores_tablero;
  while (!q.empty())
  {
    cout << endl;
    cout << "Jugador: " << q.front()->getNombre() << endl;
    cout << "Numero de territorios: " << q.front()->getNumeroTerritorios() << endl;

    for (Territorio *ter : q.front()->getTerritorios_jugador())
    {
      cout << ter->getNombre() << ": " << ter->getUnidadesDeEjercito_territorio() << endl;
    }

    cout << "Numero de tarjetas: " << q.front()->getTarjetas().size() << endl;

    for (Tarjeta *tar : q.front()->getTarjetas())
    {
      cout << tar->getTipo() << endl;
    }
    q.pop();
  }
}

int Tablero::AsignarUnidadesJugadores(vector<Jugador> jugadores)
{

  vector<Jugador> AuxJug = jugadores;

  int unidadesAux;
  if (AuxJug.size() == 2)
  {
    unidadesAux = 40;
  }
  else if (AuxJug.size() == 3)
  {
    unidadesAux = 35;
  }
  else if (AuxJug.size() == 4)
  {
    unidadesAux = 30;
  }
  else if (AuxJug.size() == 5)
  {
    unidadesAux = 25;
  }
  else if (AuxJug.size() == 6)
  {
    unidadesAux = 20;
  }

  return unidadesAux;
}

void Tablero::inicializarContinentes()
{
  Continente americaDelNorte(1, "America del Norte", "Amarillo");
  Territorio alaska(1, "Alaska", americaDelNorte.getColor());
  Territorio alberta(2, "Alberta", americaDelNorte.getColor());
  Territorio americaCen(3, "America_Central", americaDelNorte.getColor());
  Territorio estadosUnidosOri(4, "Estados_Unidos_Orientales",
                              americaDelNorte.getColor());
  Territorio groenlandia(5, "Groenlandia", americaDelNorte.getColor());
  Territorio territorioNoroc(6, "Territorio_Noroccidental",
                             americaDelNorte.getColor());
  Territorio ontario(7, "Ontario", americaDelNorte.getColor());
  Territorio quebec(8, "Quebec", americaDelNorte.getColor());
  Territorio estadosUnidosOc(9, "Estados_Unidos_Occidentales",
                             americaDelNorte.getColor());
  // Continente: America del Sur
  Continente americaDelSur(2, "America del Sur", "Rojo");
  Territorio argentina(10, "Argentina", americaDelSur.getColor());
  Territorio brasil(11, "Brasil", americaDelSur.getColor());
  Territorio peru(12, "Peru", americaDelSur.getColor());
  Territorio venezuela(13, "Venezuela", americaDelSur.getColor());
  // Continente: Europa
  Continente europa(3, "Europa", "Azul");
  Territorio granBretana(14, "Gran_Bretana", europa.getColor());
  Territorio islandia(15, "Islandia", europa.getColor());
  Territorio europaDelNorte(16, "Europa_del_Norte ", europa.getColor());
  Territorio escandinavia(17, "Escandinavia", europa.getColor());
  Territorio europaDelSur(18, "Europa_del_Sur", europa.getColor());
  Territorio ucrania(19, "Ucrania", europa.getColor());
  Territorio europaOc(20, "Europa_Occidental", europa.getColor());
  // Continente: africa
  Continente africa(4, "Africa", "Naranja");
  Territorio congo(21, "Congo", africa.getColor());
  Territorio africaOrien(22, "africa_Oriental", africa.getColor());
  Territorio egipto(23, "Egipto", africa.getColor());
  Territorio madagascar(24, "Madagascar", africa.getColor());
  Territorio africaDelNorte(25, "africa_del_Norte", africa.getColor());
  Territorio africaDelSur(26, "africa_del_Sur", africa.getColor());
  // Continente: Asia
  Continente asia(5, "Asia", "Naranja");
  Territorio afghanistan(27, "Afghanistan", asia.getColor());
  Territorio china(28, "China", asia.getColor());
  Territorio india(29, "India", asia.getColor());
  Territorio irkutsk(30, "Irkutsk", asia.getColor());
  Territorio japon(31, "Japon", asia.getColor());
  Territorio kamchatka(32, "Kamchatka", asia.getColor());
  Territorio medioOriente(33, "Medio_Oriente", asia.getColor());
  Territorio mongolia(34, "Mongolia", asia.getColor());
  Territorio siam(35, "Siam", asia.getColor());
  Territorio siberia(36, "Siberia", asia.getColor());
  Territorio ural(37, "Ural", asia.getColor());
  Territorio yakutsk(38, "Yakutsk", asia.getColor());
  // Continente: Australia
  Continente australia(6, "Australia", "Morado");
  Territorio australiaOriental(39, "Australia_Oriental", australia.getColor());
  Territorio indonesia(40, "Indonesia", australia.getColor());
  Territorio nuevaGuinea(41, "Nueva_Guinea", australia.getColor());
  Territorio australiaOc(42, "Australia_Occidental", australia.getColor());

  alaska.agregarTerritorioAlrededor(alberta);
  alaska.agregarTerritorioAlrededor(territorioNoroc);
  alaska.agregarTerritorioAlrededor(kamchatka);
  alberta.agregarTerritorioAlrededor(alaska);
  alberta.agregarTerritorioAlrededor(territorioNoroc);
  alberta.agregarTerritorioAlrededor(ontario);
  alberta.agregarTerritorioAlrededor(estadosUnidosOc);
  americaCen.agregarTerritorioAlrededor(estadosUnidosOc);
  americaCen.agregarTerritorioAlrededor(estadosUnidosOri);
  americaCen.agregarTerritorioAlrededor(venezuela);
  estadosUnidosOri.agregarTerritorioAlrededor(quebec);
  estadosUnidosOri.agregarTerritorioAlrededor(ontario);
  estadosUnidosOri.agregarTerritorioAlrededor(americaCen);
  estadosUnidosOri.agregarTerritorioAlrededor(estadosUnidosOc);
  groenlandia.agregarTerritorioAlrededor(quebec);
  groenlandia.agregarTerritorioAlrededor(ontario);
  groenlandia.agregarTerritorioAlrededor(territorioNoroc);
  groenlandia.agregarTerritorioAlrededor(islandia);
  territorioNoroc.agregarTerritorioAlrededor(groenlandia);
  territorioNoroc.agregarTerritorioAlrededor(alaska);
  territorioNoroc.agregarTerritorioAlrededor(alberta);
  territorioNoroc.agregarTerritorioAlrededor(ontario);
  ontario.agregarTerritorioAlrededor(groenlandia);
  ontario.agregarTerritorioAlrededor(territorioNoroc);
  ontario.agregarTerritorioAlrededor(quebec);
  ontario.agregarTerritorioAlrededor(alberta);
  ontario.agregarTerritorioAlrededor(estadosUnidosOc);
  ontario.agregarTerritorioAlrededor(estadosUnidosOri);
  quebec.agregarTerritorioAlrededor(groenlandia);
  quebec.agregarTerritorioAlrededor(ontario);
  quebec.agregarTerritorioAlrededor(estadosUnidosOri);
  estadosUnidosOc.agregarTerritorioAlrededor(alberta);
  estadosUnidosOc.agregarTerritorioAlrededor(ontario);
  estadosUnidosOc.agregarTerritorioAlrededor(estadosUnidosOri);
  estadosUnidosOc.agregarTerritorioAlrededor(americaCen);

  granBretana.agregarTerritorioAlrededor(islandia);
  granBretana.agregarTerritorioAlrededor(europaOc);
  granBretana.agregarTerritorioAlrededor(europaDelNorte);
  granBretana.agregarTerritorioAlrededor(escandinavia);
  islandia.agregarTerritorioAlrededor(groenlandia);
  islandia.agregarTerritorioAlrededor(granBretana);
  islandia.agregarTerritorioAlrededor(escandinavia);
  europaDelNorte.agregarTerritorioAlrededor(granBretana);
  europaDelNorte.agregarTerritorioAlrededor(escandinavia);
  europaDelNorte.agregarTerritorioAlrededor(europaDelSur);
  europaDelNorte.agregarTerritorioAlrededor(ucrania);
  europaDelNorte.agregarTerritorioAlrededor(europaOc);
  escandinavia.agregarTerritorioAlrededor(granBretana);
  escandinavia.agregarTerritorioAlrededor(europaDelNorte);
  escandinavia.agregarTerritorioAlrededor(islandia);
  escandinavia.agregarTerritorioAlrededor(ucrania);
  europaDelSur.agregarTerritorioAlrededor(ucrania);
  europaDelSur.agregarTerritorioAlrededor(europaDelNorte);
  europaDelSur.agregarTerritorioAlrededor(europaOc);
  europaDelSur.agregarTerritorioAlrededor(medioOriente);
  europaDelSur.agregarTerritorioAlrededor(africaDelNorte);
  europaDelSur.agregarTerritorioAlrededor(egipto);
  ucrania.agregarTerritorioAlrededor(escandinavia);
  ucrania.agregarTerritorioAlrededor(europaDelNorte);
  ucrania.agregarTerritorioAlrededor(europaDelSur);
  ucrania.agregarTerritorioAlrededor(afghanistan);
  ucrania.agregarTerritorioAlrededor(medioOriente);
  ucrania.agregarTerritorioAlrededor(ural);
  europaOc.agregarTerritorioAlrededor(granBretana);
  europaOc.agregarTerritorioAlrededor(africaDelNorte);
  europaOc.agregarTerritorioAlrededor(europaDelNorte);
  europaOc.agregarTerritorioAlrededor(europaDelSur);

  afghanistan.agregarTerritorioAlrededor(ural);
  afghanistan.agregarTerritorioAlrededor(china);
  afghanistan.agregarTerritorioAlrededor(india);
  afghanistan.agregarTerritorioAlrededor(medioOriente);
  china.agregarTerritorioAlrededor(afghanistan);
  china.agregarTerritorioAlrededor(india);
  china.agregarTerritorioAlrededor(siam);
  china.agregarTerritorioAlrededor(mongolia);
  china.agregarTerritorioAlrededor(siberia);
  china.agregarTerritorioAlrededor(ural);
  india.agregarTerritorioAlrededor(china);
  india.agregarTerritorioAlrededor(afghanistan);
  india.agregarTerritorioAlrededor(medioOriente);
  india.agregarTerritorioAlrededor(siam);
  irkutsk.agregarTerritorioAlrededor(siberia);
  irkutsk.agregarTerritorioAlrededor(yakutsk);
  irkutsk.agregarTerritorioAlrededor(kamchatka);
  irkutsk.agregarTerritorioAlrededor(mongolia);
  japon.agregarTerritorioAlrededor(mongolia);
  japon.agregarTerritorioAlrededor(kamchatka);
  kamchatka.agregarTerritorioAlrededor(japon);
  kamchatka.agregarTerritorioAlrededor(yakutsk);
  kamchatka.agregarTerritorioAlrededor(mongolia);
  kamchatka.agregarTerritorioAlrededor(irkutsk);
  kamchatka.agregarTerritorioAlrededor(alaska);
  medioOriente.agregarTerritorioAlrededor(afghanistan);
  medioOriente.agregarTerritorioAlrededor(india);
  medioOriente.agregarTerritorioAlrededor(europaDelSur);
  medioOriente.agregarTerritorioAlrededor(ucrania);
  medioOriente.agregarTerritorioAlrededor(egipto);
  mongolia.agregarTerritorioAlrededor(siberia);
  mongolia.agregarTerritorioAlrededor(china);
  mongolia.agregarTerritorioAlrededor(irkutsk);
  mongolia.agregarTerritorioAlrededor(japon);
  mongolia.agregarTerritorioAlrededor(kamchatka);
  siam.agregarTerritorioAlrededor(china);
  siam.agregarTerritorioAlrededor(india);
  siam.agregarTerritorioAlrededor(indonesia);
  siberia.agregarTerritorioAlrededor(china);
  siberia.agregarTerritorioAlrededor(irkutsk);
  siberia.agregarTerritorioAlrededor(mongolia);
  siberia.agregarTerritorioAlrededor(ural);
  siberia.agregarTerritorioAlrededor(yakutsk);
  ural.agregarTerritorioAlrededor(china);
  ural.agregarTerritorioAlrededor(afghanistan);
  ural.agregarTerritorioAlrededor(siberia);
  ural.agregarTerritorioAlrededor(ucrania);
  yakutsk.agregarTerritorioAlrededor(irkutsk);
  yakutsk.agregarTerritorioAlrededor(kamchatka);
  yakutsk.agregarTerritorioAlrededor(siberia);

  australiaOriental.agregarTerritorioAlrededor(nuevaGuinea);
  australiaOriental.agregarTerritorioAlrededor(australiaOc);
  indonesia.agregarTerritorioAlrededor(siam);
  indonesia.agregarTerritorioAlrededor(nuevaGuinea);
  indonesia.agregarTerritorioAlrededor(australiaOc);
  nuevaGuinea.agregarTerritorioAlrededor(australiaOriental);
  nuevaGuinea.agregarTerritorioAlrededor(indonesia);
  nuevaGuinea.agregarTerritorioAlrededor(australiaOc);
  australiaOc.agregarTerritorioAlrededor(australiaOriental);
  australiaOc.agregarTerritorioAlrededor(indonesia);
  australiaOc.agregarTerritorioAlrededor(nuevaGuinea);

  congo.agregarTerritorioAlrededor(africaOrien);
  congo.agregarTerritorioAlrededor(africaDelNorte);
  congo.agregarTerritorioAlrededor(africaDelSur);
  africaOrien.agregarTerritorioAlrededor(congo);
  africaOrien.agregarTerritorioAlrededor(egipto);
  africaOrien.agregarTerritorioAlrededor(madagascar);
  africaOrien.agregarTerritorioAlrededor(africaDelNorte);
  africaOrien.agregarTerritorioAlrededor(africaDelSur);
  egipto.agregarTerritorioAlrededor(africaOrien);
  egipto.agregarTerritorioAlrededor(africaDelNorte);
  egipto.agregarTerritorioAlrededor(europaDelSur);
  egipto.agregarTerritorioAlrededor(medioOriente);
  madagascar.agregarTerritorioAlrededor(africaOrien);
  madagascar.agregarTerritorioAlrededor(europaDelSur);
  africaDelNorte.agregarTerritorioAlrededor(congo);
  africaDelNorte.agregarTerritorioAlrededor(africaOrien);
  africaDelNorte.agregarTerritorioAlrededor(egipto);
  africaDelNorte.agregarTerritorioAlrededor(china);
  africaDelNorte.agregarTerritorioAlrededor(europaOc);
  africaDelNorte.agregarTerritorioAlrededor(europaDelSur);
  africaDelSur.agregarTerritorioAlrededor(congo);
  africaDelSur.agregarTerritorioAlrededor(africaOrien);
  africaDelSur.agregarTerritorioAlrededor(madagascar);

  argentina.agregarTerritorioAlrededor(brasil);
  argentina.agregarTerritorioAlrededor(peru);
  brasil.agregarTerritorioAlrededor(peru);
  brasil.agregarTerritorioAlrededor(argentina);
  brasil.agregarTerritorioAlrededor(venezuela);
  brasil.agregarTerritorioAlrededor(africaDelNorte);
  peru.agregarTerritorioAlrededor(argentina);
  peru.agregarTerritorioAlrededor(brasil);
  peru.agregarTerritorioAlrededor(venezuela);
  venezuela.agregarTerritorioAlrededor(brasil);
  venezuela.agregarTerritorioAlrededor(peru);
  venezuela.agregarTerritorioAlrededor(americaCen);

  americaDelNorte.agregarTerritorios(alaska);
  americaDelNorte.agregarTerritorios(alberta);
  americaDelNorte.agregarTerritorios(americaCen);
  americaDelNorte.agregarTerritorios(estadosUnidosOri);
  americaDelNorte.agregarTerritorios(groenlandia);
  americaDelNorte.agregarTerritorios(territorioNoroc);
  americaDelNorte.agregarTerritorios(ontario);
  americaDelNorte.agregarTerritorios(quebec);
  americaDelNorte.agregarTerritorios(estadosUnidosOc);
  americaDelSur.agregarTerritorios(brasil);
  americaDelSur.agregarTerritorios(peru);
  americaDelSur.agregarTerritorios(venezuela);
  americaDelSur.agregarTerritorios(argentina);
  europa.agregarTerritorios(granBretana);
  europa.agregarTerritorios(islandia);
  europa.agregarTerritorios(europaDelNorte);
  europa.agregarTerritorios(escandinavia);
  europa.agregarTerritorios(europaDelSur);
  europa.agregarTerritorios(ucrania);
  europa.agregarTerritorios(europaOc);
  africa.agregarTerritorios(congo);
  africa.agregarTerritorios(africaOrien);
  africa.agregarTerritorios(egipto);
  africa.agregarTerritorios(madagascar);
  africa.agregarTerritorios(africaDelNorte);
  africa.agregarTerritorios(africaDelSur);
  asia.agregarTerritorios(afghanistan);
  asia.agregarTerritorios(china);
  asia.agregarTerritorios(india);
  asia.agregarTerritorios(irkutsk);
  asia.agregarTerritorios(japon);
  asia.agregarTerritorios(kamchatka);
  asia.agregarTerritorios(medioOriente);
  asia.agregarTerritorios(mongolia);
  asia.agregarTerritorios(siam);
  asia.agregarTerritorios(siberia);
  asia.agregarTerritorios(ural);
  asia.agregarTerritorios(yakutsk);
  australia.agregarTerritorios(australiaOriental);
  australia.agregarTerritorios(indonesia);
  australia.agregarTerritorios(nuevaGuinea);
  australia.agregarTerritorios(australiaOc);

  continentes_tablero.push_back(americaDelNorte);
  continentes_tablero.push_back(americaDelSur);
  continentes_tablero.push_back(europa);
  continentes_tablero.push_back(africa);
  continentes_tablero.push_back(asia);
  continentes_tablero.push_back(australia);
}

void Tablero::inicializarTarjetas()

{
  vector<Territorio> terAux;
  vector<string> tipos = {"Infanteria", "Caballeria", "Artilleria"};

  for (Continente continente : continentes_tablero)
  {
    for (Territorio territorio : continente.getTerritoriosContinente())
    {
      terAux.push_back(territorio);
    }
  }

  for (int i = 0; i < terAux.size(); i++)
  {
    Tarjeta *tar = new Tarjeta(tipos[i % 3], &terAux[i]);
    tarjetas_tablero.push(tar);
  }
  Tarjeta *com1 = new Tarjeta("Comodin");
  Tarjeta *com2 = new Tarjeta("Comodin");

  tarjetas_tablero.push(com1);
  tarjetas_tablero.push(com2);
}

queue<Jugador *> Tablero::getJugadoresTablero()
{

  return jugadores_tablero;
}
stack<Tarjeta *> Tablero::getTarjetas_tablero()
{
  return tarjetas_tablero;
}
list<Continente> Tablero::getContinentes_tablero()
{
  return continentes_tablero;
}

void Tablero::setJugadoresTablero(queue<Jugador *> jugadores)
{
  jugadores_tablero = jugadores;
}

void Tablero::setTarjetasTablero(stack<Tarjeta *> tarjetas)
{
  tarjetas_tablero = tarjetas;
}

int Tablero::numeroTerritorios()
{
  int numter = 0;
  for (Continente continente : continentes_tablero)
  {
    for (Territorio territorio : continente.getTerritoriosContinente())
    {
      numter += 1;
    }
  }
  return numter;
}

void Tablero::turnoJugador(string nomJug)
{
  if (verificarJugadorTurno(nomJug))
  {
    faseDraft();
    faseAtacar();
    faseFortalecer();

    Jugador *frente = jugadores_tablero.front();
    jugadores_tablero.pop();
    jugadores_tablero.push(frente);
  }
}

bool Tablero::verificarJugadorTurno(string nomJug)
{
  queue<Jugador *> q_copia = jugadores_tablero;

  Jugador *jugador_ptr = q_copia.front();

  if (jugador_ptr != nullptr)
  {
    if (jugador_ptr->getNombre() == nomJug)
    {
      if (!jugador_ptr->getTerritorios_jugador().empty())
      {
        return true;
      }
      else
      {
        jugadores_tablero.pop();
        cout << "El jugador " << nomJug << " no tiene territorios" << endl;
        return false;
      }
    }
    else
    {
      cout << "No es el turno del jugador " << nomJug << endl;
      cout<<"$ ";
      return false;
    }
  }
  return false;
}

void Tablero::faseDraft()
{
  cout << endl;
  imprimirLineaDeLimitacion(50, '_');
  cout << "FASE DE DRAFT" << endl;
  queue<Jugador *> q_copia = jugadores_tablero;
  Jugador *jugador = q_copia.front();
  int entradaTerritorio;
  bool territorioCorrecto = false;
  int nuevasUnidades = calcularCantUnidadesNuevas(jugador);

  cout << "Tarjetas disponibles: " << jugadores_tablero.front()->getTarjetas().size() << endl;
  if (jugadores_tablero.front()->getTarjetas().size() >= 1)
  {
    for (Tarjeta *tarjeta : jugadores_tablero.front()->getTarjetas())
    {
      cout << "Tarjeta: " << tarjeta->getTipo() << endl;
    }
  }

  jugador->agregarUnidadesAdicionales(nuevasUnidades);

  while (nuevasUnidades > 0)
  {
    cout << endl;
    cout << "Jugador: " << jugador->getNombre() << endl;
    for (Territorio *territorio : jugador->getTerritorios_jugador())
    {
      cout << territorio->getId() << " " << territorio->getNombre() << ": " << territorio->getUnidadesDeEjercito_territorio() << endl;
    }

    cout << "Usted cuenta con " << nuevasUnidades << " para asignar." << endl;
    cout << "A cual territorio quiere asignarlas?: ";
    cin >> entradaTerritorio;
    for (Territorio *territorio : jugador->getTerritorios_jugador())
    {
      if (territorio->getId() == entradaTerritorio)
      {
        territorioCorrecto = true;
        bool cantidadUnidades = false;
        int unidadesIngresadas = 0;

        do
        {
          cout << "Cuantas unidades quiere asignar?: ";
          cin >> unidadesIngresadas;
          if (unidadesIngresadas > 0 && unidadesIngresadas <= nuevasUnidades)
          {
            cantidadUnidades = true;
            territorio->agregarUnidadesTerritorioAdicional(unidadesIngresadas);
            nuevasUnidades -= unidadesIngresadas;
          }
          else
          {
            cout << "Cantidad de unidades invalida" << endl;
          }
        } while (!cantidadUnidades);
      }
    }
    if (!territorioCorrecto)
    {
      cout << "El ID " << entradaTerritorio << " no es valido." << endl;
    }
  }
}

void Tablero::faseAtacar()
{
  bool entrada1Correcta = false, entrada2Correcta = false, entrada3Correcta = false;
  string entradaJugador;
  int territorioAtacar, territorioDef;
  imprimirLineaDeLimitacion(50, '_');
  cout << "\nFASE DE ATAQUE" << endl;

  vector<Jugador> AuxJug;
  queue<Jugador *> copia_jugador = jugadores_tablero;

  while (!copia_jugador.empty())
  {
    Jugador *jugador_ptr = copia_jugador.front(); // Obtener el puntero al jugador
    // Desreferenciar el puntero y agregarlo al vector
    if (jugador_ptr != nullptr)
    {
      Jugador jugador = *jugador_ptr;
      AuxJug.push_back(jugador);
      copia_jugador.pop(); // Quitar el jugador de la cola
    }
  }

  cout << "Quiere atacar algun territorio? (Si o No)" << endl;
  cin >> entradaJugador;

  while (!entrada1Correcta)
  {
    Jugador *jugAux = jugadores_tablero.front();

    for (Territorio *terActualizar : jugAux->getTerritorios_jugador())
    {
      terActualizar->setTerritorioAlrededor(actualizarTropasEnTerritorios(terActualizar->getTerritorioAlrededor()));
    }

    if (entradaJugador == "Si")
    {
      entrada1Correcta = true;
      do
      {
        cout << "Territorios de " << jugAux->getNombre() << endl;
        for (Territorio *ter : jugAux->getTerritorios_jugador())
        {
          cout << ter->getId() << ". " << ter->getNombre() << ": " << ter->getUnidadesDeEjercito_territorio() << endl;
        }
        cout << "Desde que territorio desea atacar?" << endl;
        cin >> territorioAtacar;

        for (Territorio *terAtac : jugAux->getTerritorios_jugador())
        {
          if (territorioAtacar == terAtac->getId())
          {
            if (terAtac->getUnidadesDeEjercito_territorio() > 1)
            {
              entrada2Correcta = true;
              do
              {
                cout << "Territorios para atacar" << endl;
                for (Territorio &terDef : terAtac->getTerritorioAlrededor())
                {
                  if (terAtac->getPropietario() != terDef.getPropietario())
                  {
                    cout << terDef.getId() << ". " << terDef.getNombre() << " " << terDef.getUnidadesDeEjercito_territorio() << endl;
                  }
                }
                cout << "Cual territorio desea atacar?" << endl;
                cin >> territorioDef;

                for (Territorio &terDef : terAtac->getTerritorioAlrededor())
                {
                  if (terDef.getId() == territorioDef)
                  {
                    entrada3Correcta = true;
                    imprimirLineaDeLimitacion(40, '-');
                    cout << "COMBATE" << endl;
                    int resultado = combate(terAtac->getPropietario(), terDef.getPropietario());
                    imprimirLineaDeLimitacion(40, '-');

                    if (resultado == 1)
                    {
                      terDef.eliminarUnidadesTerritorio();
                      terAtac->eliminarUnidadesTerritorio();
                      if (terDef.getUnidadesDeEjercito_territorio() == 0)
                      {
                        cout << "El territorio " << terDef.getNombre() << " ha sido conquistado por " << jugAux->getNombre() << endl;
                        Territorio *terNuevo = new Territorio(terDef.getId(), terDef.getNombre(), terDef.getColor(), terDef.getTerritorioAlrededor());
                        terNuevo->setPropietario(jugAux->getId());
                        terNuevo->setUnidadesDeEjercito_territorio(1);
                        jugAux->agregarTerritorio(terNuevo);

                        queue<Jugador *> copia_jugador = jugadores_tablero;

                        AuxJug.clear();

                        while (!copia_jugador.empty())
                        {
                          Jugador *jugador_ptr = copia_jugador.front(); // Obtener el puntero al jugador
                          // Desreferenciar el puntero y agregarlo al vector
                          if (jugador_ptr != nullptr)
                          {
                            Jugador jugador = *jugador_ptr;
                            AuxJug.push_back(jugador);
                            copia_jugador.pop(); // Quitar el jugador de la cola
                          }
                        }

                        for (Jugador &jugBusc : AuxJug)
                        {
                          if (jugBusc.getId() == terAtac->getPropietario())
                          {
                            Tarjeta *tar = tarjetas_tablero.top();
                            Tarjeta *nuevaTar = new Tarjeta(tar->getTipo(), tar->getTerritorio());
                            jugBusc.agregarTarjeta(nuevaTar);
                            tarjetas_tablero.pop();
                            cout << "Has adquirido una nueva tarjeta: " << nuevaTar->getTipo() << endl;
                          }
                        }

                        for (Jugador &jug : AuxJug)
                        {
                          if (terDef.getPropietario() == jug.getId())
                          {
                            jug.eliminarTerritorio(terDef.getId());
                            jug.setNumeroTerritorios(jug.getTerritorios_jugador().size());
                          }
                        }

                        while (!jugadores_tablero.empty())
                        {
                          delete jugadores_tablero.front();
                          jugadores_tablero.pop();
                        }

                        // Copiar elementos de AuxJug de vuelta a la cola original
                        for (const Jugador &jugador : AuxJug)
                        {
                          Jugador *jugador_ptr = new Jugador(jugador); // Crear una nueva copia del jugador
                          jugadores_tablero.push(jugador_ptr);         // Agregar la copia a la cola original
                        }
                      }
                    }
                    else if (resultado == 2 || resultado == 3)
                    {

                      terAtac->eliminarUnidadesTerritorio();
                      jugAux->eliminarUnidadesEjercito();

                      queue<Jugador *> copia_jugador = jugadores_tablero;

                      AuxJug.clear();

                      while (!copia_jugador.empty())
                      {
                        Jugador *jugador_ptr = copia_jugador.front(); // Obtener el puntero al jugador
                        // Desreferenciar el puntero y agregarlo al vector
                        if (jugador_ptr != nullptr)
                        {
                          Jugador jugador = *jugador_ptr;
                          AuxJug.push_back(jugador);
                          copia_jugador.pop(); // Quitar el jugador de la cola
                        }
                      }
                    }
                    break;
                  }
                }

                if (!entrada3Correcta)
                {
                  cout << "No es valido atacar al territorio con ID " << territorioDef << endl;
                }
              } while (!entrada3Correcta);
            }
            else
            {
              cout << "No puede atacar desde territorios con 1 tropa" << endl;
            }
            break;
          }
        }
        if (!entrada2Correcta)
        {
          cout << "El ID " << territorioAtacar << " no es valido para poder atacar" << endl;
        }

      } while (!entrada2Correcta);
    }
    else if (entradaJugador == "No")
    {
      break;
    }
    else
    {
      cout << "La entrada " << entradaJugador << " no es correcta" << endl;
    }

    cout << "Quiere volver a atacar? (Si o No)" << endl;
    cin >> entradaJugador;

    if (entradaJugador == "Si")
    {
      entrada1Correcta = false;
    }
    else if (entradaJugador == "No")
    {
      entrada1Correcta = true;
    }
    else
    {
      entrada1Correcta == false;
      cout << "La entrada " << entradaJugador << " no es valida" << endl;
    }
  }

  while (!jugadores_tablero.empty())
  {
    delete jugadores_tablero.front();
    jugadores_tablero.pop();
  }

  // Copiar elementos de AuxJug de vuelta a la cola original
  for (const Jugador &jugador : AuxJug)
  {
    Jugador *jugador_ptr = new Jugador(jugador); // Crear una nueva copia del jugador
    jugadores_tablero.push(jugador_ptr);         // Agregar la copia a la cola original
  }
}

void Tablero::faseFortalecer()
{
  queue<Jugador *> q_copia = jugadores_tablero;
  Jugador *jugador = q_copia.front();
  string entradaJugador;
  int entrada2Jugador, entrada3Jugador, entrada4Jugador;
  bool entradaCorrecta = false, entrada2Correcta = false, entrada3Correcta = false, entrada4Correcta = false;
  int totalTer = 0, unidadesAdi = 0;

  imprimirLineaDeLimitacion(50, '_');
  cout << "\nFASE DE FORTALECER" << endl;

  while (!entradaCorrecta)
  {
    for (Territorio *terActualizar : jugador->getTerritorios_jugador())
    {
      terActualizar->setTerritorioAlrededor(actualizarTropasEnTerritorios(terActualizar->getTerritorioAlrededor()));
    }
    cout << "Quiere fortificar? (Si o No)" << endl;
    cin >> entradaJugador;

    if (entradaJugador == "Si")
    {
      cout << "Territorios " << jugador->getNombre() << endl;
      for (Territorio *ter : jugador->getTerritorios_jugador())
      {
        cout << ter->getId() << ". " << ter->getNombre() << ": " << ter->getUnidadesDeEjercito_territorio() << endl;
      }
      do
      {
        cout << "Desde cual territorio quiere mandar tropas: " << endl;
        cin >> entrada2Jugador;

        for (Territorio *ter : jugador->getTerritorios_jugador())
        {
          if (entrada2Jugador == ter->getId() && ter->getUnidadesDeEjercito_territorio() > 1)
          {
            entrada2Correcta = true;

            cout << "Territorios a fortalecer: " << endl;

            for (Territorio terAlrededor : ter->getTerritorioAlrededor())
            {
              if (terAlrededor.getPropietario() == ter->getPropietario())
              {
                totalTer++;
                cout << terAlrededor.getId() << ". " << terAlrededor.getNombre() << ": " << terAlrededor.getUnidadesDeEjercito_territorio() << endl;
              }
            }
            if (totalTer == 0)
            {
              cout << "Este territorio no cuenta con territorios a los cuales fortificar" << endl;
              break;
            }
            else
            {
              do
              {
                cout << "Ingrese a el cual territorio quiere fortificar: " << endl;
                cin >> entrada3Jugador;

                for (Territorio terAlrededor : ter->getTerritorioAlrededor())
                {
                  if (terAlrededor.getPropietario() == ter->getPropietario())
                  {
                    if (entrada3Jugador == terAlrededor.getId())
                    {
                      entrada3Correcta = true;
                      do
                      {

                        cout << "Cuantas unidades de las " << ter->getUnidadesDeEjercito_territorio() << " quiere mover?" << endl;
                        cin >> entrada4Jugador;

                        if (ter->getUnidadesDeEjercito_territorio() - entrada4Jugador >= 1)
                        {
                          entrada4Correcta = true;
                          terAlrededor.agregarUnidadesTerritorioAdicional(entrada4Jugador);

                          for (int i = 0; i < entrada4Jugador; i++)
                          {
                            ter->eliminarUnidadesTerritorio();
                          }
                          entradaCorrecta = true;
                        }
                        else if (ter->getUnidadesDeEjercito_territorio() - entrada4Jugador == 0)
                        {
                          cout << "El territorio desde donde fortificamos no puede quedar con menos de 1 unidad" << endl;
                        }
                        else
                        {
                          cout << "La cantidad ingresada sobrepasa las unidades en el territorio" << endl;
                        }

                      } while (!entrada4Correcta);
                    }
                  }
                }
                if (!entrada3Correcta)
                {
                  cout << "La ID " << entrada3Jugador << " no es valida." << endl;
                }

              } while (!entrada3Correcta);
            }
          }
        }
        if (!entrada2Correcta)
        {
          cout << "El ID " << entrada2Jugador << " no es valido para mandar tropas" << endl;
        }

      } while (!entrada2Correcta);

      if (entrada4Correcta)
      {
        cout << endl;
        cout << "Su territorio se ha fortificado exitosamente" << endl;
        for (Territorio *ter : jugador->getTerritorios_jugador())
        {
          if (ter->getId() == entrada3Jugador)
          {
            ter->agregarUnidadesTerritorioAdicional(entrada4Jugador);
          }

          cout << "Territorio: " << ter->getNombre() << " - Unidades: " << ter->getUnidadesDeEjercito_territorio() << endl;
        }
      }
    }
    else if (entradaJugador == "No")
    {
      entradaCorrecta = true;
    }
    else
    {
      cout << "Entrada invalida" << endl;
    }
  }
}

int Tablero::calcularCantUnidadesNuevas(Jugador *jugAux)
{
  bool entradaCorrecta = false;
  string entradaJugador;
  int nuevasUnidades = (jugAux->getTerritorios_jugador().size()) / 3;
  if (jugAux->getTarjetas().size() > 2 && jugAux->getTarjetas().size() < 5)
  {
    do
    {
      cout << "Desea cambiar cartas? (Si o No): ";
      cin >> entradaJugador;
      if (entradaJugador == "Si")
      {
        nuevasUnidades += calcularCantUnidadesCartas(jugAux);
        entradaCorrecta = true;
        return nuevasUnidades;
      }
      else if (entradaJugador == "No")
      {
        entradaCorrecta = true;
      }

    } while (!entradaCorrecta);
  }
  else if (jugAux->getTarjetas().size() >= 5)
  {
    nuevasUnidades += calcularCantUnidadesCartas(jugAux);
    return nuevasUnidades;
  }
  return nuevasUnidades;
}

int Tablero::calcularCantUnidadesCartas(Jugador *jugAux)
{
  int nuvUnidades = 0;
  int numCambios = 0;

  map<string, int> contadorTarjetas;
  vector<Tarjeta *> &tarjetas_jugador = jugAux->getTarjetas(); // Usar una referencia al vector de tarjetas

  for (Tarjeta *tarjeta : jugAux->getTarjetas())
  {
    contadorTarjetas[tarjeta->getTipo()]++;
  }

  if (contadorTarjetas["Infanteria"] >= 3 ||
      contadorTarjetas["Caballeria"] >= 3 ||
      contadorTarjetas["Artilleria"] >= 3 ||
      (contadorTarjetas["Infanteria"] >= 1 && contadorTarjetas["Caballeria"] >= 1 && contadorTarjetas["Artilleria"] >= 1) ||
      (contadorTarjetas["Infanteria"] >= 1 && contadorTarjetas["Artilleria"] >= 1 && contadorTarjetas["Comodin"] >= 1) ||
      (contadorTarjetas["Caballeria"] >= 1 && contadorTarjetas["Artilleria"] >= 1 && contadorTarjetas["Comodin"] >= 1) ||
      (contadorTarjetas["Infanteria"] >= 1 && contadorTarjetas["Caballeria"] >= 1 && contadorTarjetas["Comodin"] >= 1) ||
      (contadorTarjetas["Infanteria"] >= 2 && contadorTarjetas["Comodin"] >= 1) ||
      (contadorTarjetas["Caballeria"] >= 2 && contadorTarjetas["Comodin"] >= 1) ||
      (contadorTarjetas["Artilleria"] >= 2 && contadorTarjetas["Comodin"] >= 1))
  {
    jugAux->setTarjetasCambiadas(jugAux->getTarjetasCambiadas() + 1);
    numCambios = jugAux->getTarjetasCambiadas();
    if (numCambios == 1)
    {
      nuvUnidades = 4;
    }
    else if (numCambios == 2)
    {
      nuvUnidades = 6;
    }
    else if (numCambios == 3)
    {
      nuvUnidades = 8;
    }
    else if (numCambios == 4)
    {
      nuvUnidades = 10;
    }
    else if (numCambios == 5)
    {
      nuvUnidades = 12;
    }
    else if (numCambios == 6)
    {
      nuvUnidades = 15;
    }
    else if (numCambios == 7)
    {
      nuvUnidades = 20;
    }
    else if (numCambios == 8)
    {
      nuvUnidades = 25;
    }
    else
    {
      nuvUnidades = 30;
    }

    // Eliminar las tarjetas usadas
    if (contadorTarjetas["Infanteria"] >= 3)
    {
      eliminarTarjetas(tarjetas_jugador, "Infanteria", 3);
    }
    else if (contadorTarjetas["Caballeria"] >= 3)
    {
      eliminarTarjetas(tarjetas_jugador, "Caballeria", 3);
    }
    else if (contadorTarjetas["Artilleria"] >= 3)
    {
      eliminarTarjetas(tarjetas_jugador, "Artilleria", 3);
    }
    else if (contadorTarjetas["Infanteria"] >= 1 && contadorTarjetas["Caballeria"] >= 1 && contadorTarjetas["Artilleria"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Infanteria", 1);
      eliminarTarjetas(tarjetas_jugador, "Caballeria", 1);
      eliminarTarjetas(tarjetas_jugador, "Artilleria", 1);
    }
    else if (contadorTarjetas["Infanteria"] >= 1 && contadorTarjetas["Artilleria"] >= 1 && contadorTarjetas["Comodin"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Infanteria", 1);
      eliminarTarjetas(tarjetas_jugador, "Artilleria", 1);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 1);
    }
    else if (contadorTarjetas["Caballeria"] >= 1 && contadorTarjetas["Artilleria"] >= 1 && contadorTarjetas["Comodin"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Caballeria", 1);
      eliminarTarjetas(tarjetas_jugador, "Artilleria", 1);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 1);
    }
    else if (contadorTarjetas["Infanteria"] >= 1 && contadorTarjetas["Caballeria"] >= 1 && contadorTarjetas["Comodin"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Infanteria", 1);
      eliminarTarjetas(tarjetas_jugador, "Caballeria", 1);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 1);
    }
    else if (contadorTarjetas["Artilleria"] >= 2 && contadorTarjetas["Comodin"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Artilleria", 2);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 1);
    }
    else if (contadorTarjetas["Infanteria"] >= 2 && contadorTarjetas["Comodin"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Infanteria", 2);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 1);
    }
    else if (contadorTarjetas["Caballeria"] >= 2 && contadorTarjetas["Comodin"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Caballeria", 2);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 1);
    }
    else if (contadorTarjetas["Comodin"] >= 2 && contadorTarjetas["Caballeria"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Caballeria", 1);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 2);
    }
    else if (contadorTarjetas["Comodin"] >= 2 && contadorTarjetas["Infanteria"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Infanteria", 1);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 2);
    }
    else if (contadorTarjetas["Comodin"] >= 2 && contadorTarjetas["Artilleria"] >= 1)
    {
      eliminarTarjetas(tarjetas_jugador, "Artilleria", 1);
      eliminarTarjetas(tarjetas_jugador, "Comodin", 2);
    }

    return nuvUnidades;
  }
  else
  {
    cout << "No se encontro ninguna combinacion valida." << endl;
    return nuvUnidades;
  }
}

void Tablero::eliminarTarjetas(vector<Tarjeta *> &tarjetas_jugador, const string &tipo, int cantidad)
{
  for (auto it = tarjetas_jugador.begin(); it != tarjetas_jugador.end();)
  {
    if ((*it)->getTipo() == tipo && cantidad > 0)
    {
      delete *it;
      it = tarjetas_jugador.erase(it);
      cantidad--;
    }
    else
    {
      ++it;
    }
  }
}

int Tablero::tirarDado()
{
  static bool seeded = false;
  if (!seeded)
  {
    srand(static_cast<unsigned int>(time(nullptr)));
    seeded = true;
  }

  return rand() % 6 + 1;
}

int Tablero::combate(string idAtacante, string idDefensor)
{
  vector<int> resultadosAtacante;
  vector<int> resultadosDefensor;

  for (int i = 0; i < 3; i++)
  {
    resultadosAtacante.push_back(tirarDado());
  }

  for (int i = 0; i < 2; i++)
  {
    resultadosDefensor.push_back(tirarDado());
  }

  sort(resultadosAtacante.rbegin(), resultadosAtacante.rend());
  sort(resultadosDefensor.rbegin(), resultadosDefensor.rend());

  std::cout << "Jugador " << idAtacante << " (Atacante) tira los dados: ";
  for (int resultado : resultadosAtacante)
  {
    cout << resultado << " ";
  }
  cout << endl;

  cout << "Jugador " << idDefensor << " (Defensor) tira los dados: ";
  for (int resultado : resultadosDefensor)
  {
    cout << resultado << " ";
  }
  cout << endl;

  int mejorAtacante = resultadosAtacante[0];
  int segundoMejorAtacante = resultadosAtacante[1];
  int mejorDefensor = resultadosDefensor[0];
  int segundoMejorDefensor = resultadosDefensor[1];

  if (mejorAtacante > mejorDefensor && segundoMejorAtacante > segundoMejorDefensor)
  {
    cout << "El Jugador " << idAtacante << " gana" << endl;
    return 1;
  }
  else if (mejorDefensor > mejorAtacante && segundoMejorDefensor > segundoMejorAtacante)
  {
    cout << "El Jugador " << idDefensor << " gana" << endl;
    return 2;
  }
  else
  {
    cout << "Es un empate" << endl;
    return 3;
  }
}

vector<Territorio> Tablero ::actualizarTropasEnTerritorios(vector<Territorio> v)
{
  queue<Jugador *> q_copia = jugadores_tablero;
  vector<Jugador *> jugadoresCopia;
  vector<Territorio *> territoriosConTropas;

  while (!q_copia.empty())
  {
    jugadoresCopia.push_back(q_copia.front());
    q_copia.pop();
  }

  for (Jugador *jugador : jugadoresCopia)
  {
    for (Territorio *territorio : jugador->getTerritorios_jugador())
    {
      territoriosConTropas.push_back(territorio);
    }
  }

  for (Territorio *ter : territoriosConTropas)
  {
    for (Territorio &ter2 : v)
    {
      if (ter2.getId() == ter->getId())
      {
        ter2.setPropietario(ter->getPropietario());
        ter2.setUnidadesDeEjercito_territorio(ter->getUnidadesDeEjercito_territorio());
      }
    }
  }

  return v;
}

void Tablero::imprimirLineaDeLimitacion(int ancho, char simbolo)
{
  string linea(ancho, simbolo); // Crear una cadena de caracteres con el ancho especificado y llenarla con guiones (-)
  cout << linea << std::endl;   // Imprimir la línea
}