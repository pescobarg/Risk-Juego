#include "lectorArchivo.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <bitset>

using namespace std;

Tablero LectorArchivo::leerArchivoTXT(Tablero tablero, string nombreArchivo, bool &inicializado)
{
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        return tablero; // No se pudo abrir el archivo
    }

    tablero.inicializarContinentes();
    tablero.inicializarTarjetas();

    queue<Jugador *> copia_jugador;
    vector<Jugador *> jugadoresCopia;

    while (!copia_jugador.empty())
    {
        jugadoresCopia.push_back(copia_jugador.front());
        copia_jugador.pop();
    }

    string linea, token;
    int totalTarjetas = 0;

    int cantidadJugadores = 0, cantidadTarjetas = 0, cantidadTerritorio = 0;

    getline(archivo, linea);
    cantidadJugadores = stoi(linea);

    for (int i = 0; i < cantidadJugadores; i++)
    {
        getline(archivo, linea);
        string nom_jug = linea;
        getline(archivo, linea);
        string id_jug = linea;
        getline(archivo, linea);
        string col_jug = linea;
        getline(archivo, linea);

        Jugador *jugNuevo = new Jugador(nom_jug, col_jug, id_jug);

        jugNuevo->setUnidadesDeEjercito(stoi(linea));
        getline(archivo, linea);

        int NumTer = stoi(linea);
        jugNuevo->setNumeroTerritorios(NumTer);
        getline(archivo, linea);

        jugNuevo->setTarjetasCambiadas(stoi(linea));

        getline(archivo, linea);
        cantidadTarjetas = stoi(linea);

        for (int j = 0; j < cantidadTarjetas; j++)
        {
            getline(archivo, linea);
            string tipo = linea;
            Tarjeta *tarjeta = new Tarjeta(tipo);

            jugNuevo->agregarTarjeta(tarjeta);
            totalTarjetas++;
        }

        for (int l = 0; l < NumTer; l++)
        {
            vector<string> datos;
            getline(archivo, linea);
            istringstream ss(linea);
            while (getline(ss, token, ','))
            {
                datos.push_back(token);
            }

            Territorio *ter = new Territorio(stoi(datos[0]), "", "");
            ter->setPropietario(id_jug);
            ter->setUnidadesDeEjercito_territorio(stoi(datos[1]));

            jugNuevo->agregarTerritorio(ter);
        }

        jugadoresCopia.push_back(jugNuevo);
    }

    list<Continente> continentes_copia = tablero.getContinentes_tablero();

    vector<Territorio> TerritorioDisponible;
    for (Continente continente : continentes_copia)
    {
        for (Territorio territorio : continente.getTerritoriosContinente())
        {
            TerritorioDisponible.push_back(territorio);
        }
    }

    for (Jugador *jug : jugadoresCopia)
    {
        for (Territorio *ter : jug->getTerritorios_jugador())
        {
            for (Territorio terCont : TerritorioDisponible)
            {
                if (ter->getId() == terCont.getId())
                {
                    ter->setNombre(terCont.getNombre());
                    ter->setTerritorioAlrededor(terCont.getTerritorioAlrededor());
                }
            }
        }
    }

    stack<Tarjeta *> tarjetas_copia = tablero.getTarjetas_tablero();

    for (int i = 0; i < totalTarjetas; i++)
    {
        tarjetas_copia.pop();
    }

    for (Jugador *jugador_ptr : jugadoresCopia)
    {
        copia_jugador.push(jugador_ptr);
    }
    tablero.setJugadoresTablero(copia_jugador);
    tablero.setTarjetasTablero(tarjetas_copia);

    archivo.close();
    inicializado = true;
    return tablero;
}

Tablero LectorArchivo::leerArchivoBin(Tablero tablero, string nombreArchivo, bool &inicializado)
{

    ifstream archivo(nombreArchivo, ios::binary);

    if (!archivo.is_open())
    {
        return tablero; // No se pudo abrir el archivo
    }

    short n;
    archivo.read(reinterpret_cast<char *>(&n), sizeof(short));
    long f;
    char c;
    map<char, long> frecuencia;

    for (int i = 0; i < n; i++)
    {
        archivo.read(reinterpret_cast<char *>(&c), sizeof(char));
        archivo.read(reinterpret_cast<char *>(&f), sizeof(long));
        frecuencia[c] = f;
    }

    long w;
    archivo.read(reinterpret_cast<char *>(&w), sizeof(long));
    string textoCod = "";
    char byte;

    while (archivo.read(&byte, sizeof(byte)))
    {
        std::bitset<8> bits(byte);
        textoCod += bits.to_string();
    }

    NodoHuffman *arbolHuffman = nullptr;
    map<char, string> codigosHuff;
    codigosHuff = construirArbolHuffmanConSecuenciasDeBits(arbolHuffman, frecuencia);

    int index = -1;

    string textoDecod;

    while (index < ((int)textoCod.size() - 2))
    {
        if (textoDecod.size() == w)
        {
            break;
        }
        decodificarCodigosHuffman(arbolHuffman, textoCod, index, textoDecod);
    }

    archivo.close();

    tablero.inicializarContinentes();
    tablero.inicializarTarjetas();

    queue<Jugador *> copia_jugador;
    vector<Jugador *> jugadoresCopia;

    while (!copia_jugador.empty())
    {
        jugadoresCopia.push_back(copia_jugador.front());
        copia_jugador.pop();
    }

    istringstream stream(textoDecod);
    string numJugadoresLinea;
    getline(stream, numJugadoresLinea, '\n');
    int totalTarjetas = 0;
    int numJugadores = stoi(numJugadoresLinea);
    for (int i = 0; i < numJugadores; i++)
    {
        int cantidadTarjetas = 0, cantidadTerritorio = 0;
        string linea;
        getline(stream, linea, '\n'); // Leer la línea del jugador

        istringstream jugadorStream(linea);
        string jugadorInfo;
        getline(jugadorStream, jugadorInfo, ';'); // Separar por ;

        string datosJugador;
        istringstream infoStream(jugadorInfo);
        getline(infoStream, datosJugador, ',');
        string nom_jug = datosJugador;

        getline(infoStream, datosJugador, ',');
        string id_jug = datosJugador;

        getline(infoStream, datosJugador, ',');
        string col_jug = datosJugador;

        Jugador *jugNuevo = new Jugador(nom_jug, col_jug, id_jug);

        getline(infoStream, datosJugador, ',');
        string unidades_jug = datosJugador;
        jugNuevo->setUnidadesDeEjercito(stoi(unidades_jug));

        getline(infoStream, datosJugador, ',');
        cantidadTerritorio = stoi(datosJugador);
        jugNuevo->setNumeroTerritorios(cantidadTerritorio);

        getline(infoStream, datosJugador, ',');
        string tarjetas_cambiadas = datosJugador;
        jugNuevo->setTarjetasCambiadas(stoi(tarjetas_cambiadas));

        getline(infoStream, datosJugador, ',');
        cantidadTarjetas = stoi(datosJugador);

        getline(jugadorStream, jugadorInfo, ';'); // Separar por ;
        string infoCartas;
        istringstream cartasStream(jugadorInfo);
        for (int j = 0; j < cantidadTarjetas; j++)
        {
            getline(cartasStream, infoCartas, ',');
            string tipo = infoCartas;
            Tarjeta *tarjeta = new Tarjeta(tipo);
            //cout << "Tarjeta: " << tipo << endl;
            jugNuevo->agregarTarjeta(tarjeta);
            totalTarjetas++;
        }

        getline(jugadorStream, jugadorInfo, ';'); // Separar por ;
        string infoTerritoios, token;
        istringstream territoriosStream(jugadorInfo);
        for (int x = 0; x < cantidadTerritorio; x++)
        {
            vector<string> datos;
            getline(territoriosStream, infoTerritoios, ',');
            //cout << "Completo ter jugador: " << infoTerritoios << endl;

            istringstream ss(infoTerritoios);
            while (getline(ss, token, '-'))
            {
                datos.push_back(token);
            }

            if (datos.size() == 2)
            {

                Territorio *ter = new Territorio(stoi(datos[0]), "", "");
                ter->setPropietario(id_jug);
                ter->setUnidadesDeEjercito_territorio(stoi(datos[1]));
                jugNuevo->agregarTerritorio(ter);
            }
            else
            {
                cout << "Error: Datos de territorio incorrectos." << endl;
            }
        }
        jugadoresCopia.push_back(jugNuevo);
    }

    list<Continente> continentes_copia = tablero.getContinentes_tablero();

    vector<Territorio> TerritorioDisponible;
    for (Continente continente : continentes_copia)
    {
        for (Territorio territorio : continente.getTerritoriosContinente())
        {
            TerritorioDisponible.push_back(territorio);
        }
    }

    for (Jugador *jug : jugadoresCopia)
    {

        for (Territorio *ter : jug->getTerritorios_jugador())
        {

            for (Territorio terCont : TerritorioDisponible)
            {

                if (ter->getId() == terCont.getId())
                {

                    ter->setNombre(terCont.getNombre());
                    ter->setTerritorioAlrededor(terCont.getTerritorioAlrededor());
                }
            }
        }
    }

    stack<Tarjeta *> tarjetas_copia = tablero.getTarjetas_tablero();

    for (int i = 0; i < totalTarjetas; i++)
    {
        tarjetas_copia.pop();
    }

    for (Jugador *jugador_ptr : jugadoresCopia)
    {
        copia_jugador.push(jugador_ptr);
    }

    tablero.setJugadoresTablero(copia_jugador);
    tablero.setTarjetasTablero(tarjetas_copia);

    inicializado = true;
    return tablero;
}