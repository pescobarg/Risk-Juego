// REALIZADO POR SERGIO ASENCIO Y PABLO JAVIER ESCOBAR
#include "comando.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const vector<Comando> comandos = {
    {"inicializar",
     "Este comando se utiliza para comenzar una nueva partida del juego Risk. "
     "Solicita la cantidad de jugadores y sus nombres, y luego les permite "
     "ubicar sus unidades de ejército en los territorios de manera "
     "consecutiva.\nPara conocer información sobre inicializar desde un "
     "archivo ingresa {ayuda inicializar_a}"},
    {"turno",
     "Este comando permite al jugador con el ID especificado realizar su "
     "turno. Durante su turno, el jugador puede obtener unidades adicionales, "
     "atacar territorios enemigos y fortificar sus posiciones.\n  -Entrada: "
     "turno <id_jugador> \n  -Ejemplo: turno 2"},
    {"salir", " Este comando se utiliza para terminar la ejecución del juego y "
              "salir de la aplicación."},
    {"guardar",
     "Guarda el estado actual del juego en un archivo de texto con el nombre "
     "especificado. El archivo contendrá información sobre los jugadores, "
     "territorios, cartas y otros detalles del juego.\n  -Entrada: guardar "
     "<nombre_archivo> \n  -Ejemplo: guardar partida1.txt"},
    {"guardar_comprimido",
     "Guarda el estado actual del juego en un archivo binario comprimido.\n  "
     "-Entrada: guardar_comprimido <nombre_archivo> \n  -Ejemplo: "
     "guardar_comprimido partida1.bin"},
    {"inicializar_a",
     "Inicializa el juego con los datos contenidos en el archivo especificado. "
     "Puede ser un archivo generado por el comando guardar o un archivo "
     "binario generado por el comando guardar_comprimido\n  -Entrada: "
     "inicializar <nombre_archivo> \n  -Ejemplo: inicializar "
     "partida_guardada.txt"},
    {"costo_conquista",
     "Calcula el costo y la secuencia de territorios a ser conquistados para "
     "controlar el territorio dado. El programa determinará el territorio "
     "desde el cual el jugador tiene el control más cercano al territorio "
     "objetivo.\n  -Entrada: costo_conquista <territorio> \n  -Ejemplo: "
     "costo_conquista Brasil"},
    {"conquista_mas_barata",
     "Calcula el territorio que puede ser conquistado con el menor número de "
     "unidades de ejército perdidas. Se evalúan todas las posibles conquistas "
     "y se selecciona la que implica un menor riesgo."},
    {"ayuda", "Muestra informacion general o especifica sobre los comandos."},
};

void ayuda(string comando_especifico) {
  if (!comando_especifico.empty()) {
    for (Comando com : comandos) {
      if (com.nombre == comando_especifico) {
        cout << " " << com.nombre << ": " << com.descripcion << endl;
        return;
      }
    }
    cout << "No es un comando valido para brindar ayuda" << endl;
  } else {
    cout << "Listado de comandos disponibles" << endl;
    for (Comando com : comandos) {
      cout << "$ " << com.nombre << endl;
    }
  }
}

bool ComandoInicializar(string argumento) {
  return argumento.empty() || argumento.size() > 0;
}

bool ComandoGuardar(string argumento) { return !argumento.empty(); }

bool validarComandoTurno(string argumento) { return !argumento.empty(); }

bool ComandoGuardarComprimido(string argumento) {
  return !argumento.empty();
}

bool ComandoCostoConquista(string argumento) {
  return !argumento.empty();
}

bool inicializarSinArgumento(int &numjug) {
   cout << "Comando 'inicializar ingresado con exito."
       << endl;
       return true;
}

void inicializarConArgumento(const string &argumento) {
  cout << "Comando 'inicializar " << argumento << "' ingresado con exito."
       << endl;
}