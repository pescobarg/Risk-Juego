// REALIZADO POR SERGIO ASENCIO Y PABLO JAVIER ESCOBAR

#include "comando.h"
#include "tablero.h"
#include "generadorArchivo.h"
#include "lectorArchivo.h"
#include "grafo.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#define REINICIAR_FORMATO "\033[0m"
#define TEXTO_VERDE "\033[32m"
#endif

using namespace std;

void PonerSimbolo(const string &textoSimbolo)
{
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
#else
  cout << TEXTO_VERDE;
#endif

  cout << textoSimbolo;

#ifndef _WIN32
  cout << REINICIAR_FORMATO;
#endif
}

int main()
{
  string entradaUsuario;
  int numJugadores;
  bool inicializado = false, imprimirSimbolo = false;
  Tablero tablero;
  PonerSimbolo("$ ");

  while (true)
  {

    if (imprimirSimbolo)
    {
      PonerSimbolo("$ ");
    }
    imprimirSimbolo = true;

    getline(cin, entradaUsuario);

    istringstream iss(entradaUsuario);
    string comando, argumento;

    if (iss >> comando)
    {
      if (comando == "salir")
      {
        cout << "Comando ingresado con exito." << endl;
        break;
      }
      else if (comando == "ayuda")
      {
        iss >> argumento;
        ayuda(argumento);
      }
      else if (comando == "inicializar")
      {
        if (iss >> argumento)
        {
          if (ComandoInicializar(argumento))
          {
            if (argumento.empty())
            {
            }
            else
            {
              size_t puntoPos = argumento.find_last_of('.');
              if (puntoPos == string::npos)
              {
                cout << "Error: Extension de archivo desconocida." << endl;
              }
              else
              {
                string extension = argumento.substr(puntoPos);

                if (extension == ".txt" && !inicializado)
                {
                  tablero = LectorArchivo::leerArchivoTXT(tablero, argumento, inicializado);
                  if (inicializado)
                  {
                    cout << "Informacion general de la partida" << endl;
                    tablero.imprimirJugadores();
                    cout << "Archivo " << argumento << " leido con exito." << endl;
                  }
                  else
                  {
                    cout << "Archivo " << argumento << " no encontrado." << endl;
                  }
                }
                else if (extension == ".bin" && !inicializado)
                {
                  tablero = LectorArchivo::leerArchivoBin(tablero, argumento, inicializado);
                  if (inicializado)
                  {
                    cout << "Informacion general de la partida" << endl;
                    tablero.imprimirJugadores();
                    cout << "Archivo " << argumento << " leido con exito." << endl;
                  }
                  else
                  {
                    cout << "Archivo " << argumento << " no encontrado." << endl;
                  }
                }
                else if (inicializado)
                {
                  cout << "Actualmente hay una partida en curso" << endl;
                }
              }
            }
          }
          else
          {
            cout << "Error: Argumento de comando 'inicializar' invalido."
                 << endl;
          }
        }
        else
        {
          if (!inicializado)
          {
            tablero.inicializarJuego();
            inicializado = true;
            imprimirSimbolo = false;
          }
          else
          {
            cout << "Actualmente hay una partida en curso" << endl;
          }
        }
      }
      else if (comando == "guardar")
      {
        if (iss >> argumento && ComandoGuardar(argumento))
        {
          size_t puntoPos = argumento.find_last_of('.');
          if (puntoPos == string::npos)
          {
            cout << "Por favor, ingrese la extension .txt luego del nombre deseado" << endl;
          }
          else
          {
            if (!inicializado)
            {
              cout << "Esta partida no ha sido inicializada correctamente." << endl;
            }
            else
            {
              GenerarArchivo::generarArchivoTXT(tablero, argumento);
              cout << " La partida ha sido codificada y guardada correctamente en el archivo " << argumento << endl;
            }
          }
        }
        else
        {
          cout << "Error: Comando invalido o argumento incorrecto." << endl;
        }
      }
      else if (comando == "guardar_comprimido")
      {
        if (iss >> argumento && ComandoGuardarComprimido(argumento))
        {
          size_t puntoPos = argumento.find_last_of('.');
          if (puntoPos == string::npos)
          {
            cout << "Por favor, ingrese la extension .bin luego del nombre deseado" << endl;
          }
          else
          {
            if (!inicializado)
            {
              cout << "Esta partida no ha sido inicializada correctamente." << endl;
            }
            else
            {
              GenerarArchivo::generarArchivoBin(tablero, argumento);
              cout << " La partida ha sido codificada y guardada correctamente en el archivo " << argumento << endl;
            }
          }
        }
        else
        {
          cout << "Error: Comando invalido o argumento incorrecto." << endl;
        }
      }
      else if (comando == "turno")
      {
        if (iss >> argumento && validarComandoTurno(argumento))
        {
          if (inicializado)
          {
            tablero.turnoJugador(argumento);
            imprimirSimbolo = false; 
          }
          else
          {
            cout << "No hay ninguna partida inicializada" << endl;
          }
        }
        else
        {
          cout << "Error: Comando invalido o argumento incorrecto." << endl;
        }
      }
      else if (comando == "costo_conquista")
      {
        if (iss >> argumento && ComandoCostoConquista(argumento))
        {

          if (inicializado)
          {
            GrafoAdyacencia grafo(tablero.numeroTerritorios());
            grafo.costo_conquista(tablero, argumento);
          }
          else
          {
            cout << "El juego no ha sido inicializado" << endl;
          }
        }
        else
        {
          cout << "Error: Comando invalido o argumento incorrecto." << endl;
        }
      }
      else if (comando == "conquista_mas_barata")
      {
        if (inicializado)
        {
          GrafoAdyacencia grafo(tablero.numeroTerritorios());
          grafo.conquista_barata(tablero);
        }
        else
        {
          cout << "El juego no ha sido inicializado" << endl;
        }
      }
      else
      {
        cout << "Error: Comando invalido" << endl;
      }
    }
  }
  return 0;
}