#include "grafo.h"
#include <limits>
#include <math.h>

using namespace std;

GrafoAdyacencia::GrafoAdyacencia(int tamano)
{
    nmbVertices = 0;
    maxVertices = tamano;
    for (int i = 0; i < tamano; i++)
    {
        vector<float> vectorAux;
        vector<int> vectorIntAux;
        aristas.push_back(vectorAux);
        visitados.push_back(false);
        dist.push_back(vectorAux);
        camino.push_back(vectorIntAux);
    }

    for (int i = 0; i < tamano; i++)
    {
        for (int j = 0; j < tamano; j++)
        {
            aristas[i].push_back(ARISTA_NULA);
            dist[i].push_back(numeric_limits<float>::max());
            camino[i].push_back(numeric_limits<int>::max());
        }
    }
}

bool GrafoAdyacencia::es_vacio()
{
    if (nmbVertices == 0)
        return true;
    else
        return false;
}

void GrafoAdyacencia::conquista_barata(Tablero tablero)
{
    llenar_matriz(tablero);
    FloydWarshall(tablero);
}
void GrafoAdyacencia::costo_conquista(Tablero tablero, string territorioHasta)
{
    vertice *verBuscado;
    if (verificarConquista(tablero, territorioHasta))
    {
        llenar_matriz(tablero);
        verBuscado = buscarVertice(territorioHasta);
        if (verBuscado != nullptr)
        {
            Dijkstra(verBuscado, tablero);
        }
    }
}

vertice *GrafoAdyacencia::buscarVertice(string nombreVer)
{
    for (vertice *v : vertices)
    {
        if (v->getTerritorio().getNombre() == nombreVer)
        {
            return v;
        }
    }
    return nullptr;
}

bool GrafoAdyacencia::verificarConquista(Tablero tab, string terBuscado)
{
    queue<Jugador *> jugadores = tab.getJugadoresTablero();
    vector<Jugador *> jugadoresVector;
    Jugador *jugadorActual = tab.getJugadoresTablero().front();

    bool territorioEncontrado = false;
    bool jugadorCorrecto = false;

    while (!jugadores.empty())
    {
        jugadoresVector.push_back(jugadores.front());
        jugadores.pop();
    }

    for (Jugador *jug : jugadoresVector)
    {
        for (Territorio *ter : jug->getTerritorios_jugador())
        {
            if (ter->getNombre() == terBuscado)
            {
                territorioEncontrado = true;
                if (jug->getId() != jugadorActual->getId())
                {
                    jugadorCorrecto = true;
                    return true;
                }
            }
        }
    }
    if (!territorioEncontrado)
    {
        cout << "El territorio " << terBuscado << " no existe." << endl;
        return false;
    }
    if (!jugadorCorrecto)
    {
        cout << "El territorio " << terBuscado << " hace parte de los territorios del jugador actual" << endl;
        return false;
    }
    return false;
}

void GrafoAdyacencia::llenar_matriz(Tablero tab)
{

    queue<Jugador *> jugadores = tab.getJugadoresTablero();
    vector<Jugador *> jugadoresVector;
    int contador = 0;

    while (!jugadores.empty())
    {
        jugadoresVector.push_back(jugadores.front());
        jugadores.pop();
    }

    for (Jugador *jug : jugadoresVector)
    {
        for (Territorio *ter : jug->getTerritorios_jugador())
        {
            vertice *nuevoVertice = new vertice(*ter);
            agregar_vertice(nuevoVertice);
            contador += 1;
        }
    }
    for (vertice *v : vertices)
    {
        for (Territorio ter : v->getTerritorio().getTerritorioAlrededor())
        {
            vertice *verticeTer = encontrarVerticeCorrespondiente(ter);
            if (verticeTer != nullptr)
            {
                agregar_arista(v, verticeTer);
            }
        }
    }
}

vertice *GrafoAdyacencia::encontrarVerticeCorrespondiente(Territorio &territorioBuscado)
{
    // Esta funcion lo que hace es buscar en el vector vertices del grafo uno que tenga un id igual al territorio alrededor que buscamos al hacer esto evitamos trabajar
    // evitamos trabajar con copias y todo lo hacemos con apuntadores

    for (vertice *v : vertices)
    {
        if (v->getTerritorio().getId() == territorioBuscado.getId())
        {
            return v;
        }
    }

    return nullptr;
}

bool GrafoAdyacencia::grafo_lleno()
{
    if (nmbVertices == maxVertices)
        return true;
    else
        return false;
}

void GrafoAdyacencia::agregar_vertice(vertice *auxVer)
{
    vertices.push_back(auxVer);
    nmbVertices++;
}

void GrafoAdyacencia::agregar_arista(vertice *desdeVertice, vertice *haciaVertice)
{
    float peso = haciaVertice->getTerritorio().getUnidadesDeEjercito_territorio();

    int indiceDesde = getIndice(desdeVertice);
    int indiceHacia = getIndice(haciaVertice);

    if (indiceDesde != -1 && indiceHacia != -1)
    {
        aristas[indiceDesde][indiceHacia] = peso;
    }
    else
    {
        cout << "Error: No se encontró el índice de uno de los vértices." << endl;
    }
}

int GrafoAdyacencia::getIndice(vertice *auxVer)
{
    int i = 0;
    while (i < nmbVertices)
    {
        if (vertices[i] == auxVer)
            return i;
        i++;
    }
    return -1;
}

void GrafoAdyacencia::vaciar_visitados()
{
    for (int i = 0; i < maxVertices; i++)
        visitados[i] = false;
}

GrafoAdyacencia::~GrafoAdyacencia() {}

void GrafoAdyacencia::imprimir_aristas()
{
    for (int i = 0; i < aristas.size(); i++)
    {
        for (int j = 0; j < aristas.size(); j++)
        {
            cout << aristas[i][j]
                 << "\t";
        }
        cout << endl;
    }
}

void GrafoAdyacencia::FloydWarshall(Tablero tab)
{
    Jugador *jugadorActual = tab.getJugadoresTablero().front();
    inicializarFloydWarshall();

    for (int k = 0; k < nmbVertices; k++)
    {
        for (int i = 0; i < nmbVertices; i++)
        {
            for (int j = 0; j < nmbVertices; j++)
            {

                if (dist[i][k] == numeric_limits<float>::max() || dist[k][j] == numeric_limits<float>::max())
                {
                    continue;
                }

                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    camino[i][j] = camino[i][k];
                }
            }
        }
    }

    vector<int> path;
    int distanciaMenor = numeric_limits<int>::max(), indiceDesde = 0, indiceHacia = 0;
    bool cambio = false;

    for (int i = 0; i < nmbVertices; i++)
    {
        if (vertices[i]->getTerritorio().getPropietario() != jugadorActual->getId())
        {

            continue;
        }

        for (int j = 0; j < nmbVertices; j++)
        {
            if (vertices[j]->getTerritorio().getPropietario() == jugadorActual->getId())
            {
                continue;
            }

            int indice1 = getIndicePorID(vertices[i]->getTerritorio().getId());
            int indice2 = getIndicePorID(vertices[j]->getTerritorio().getId());
            path = construirCamino(indice1, indice2);

            int distanciaNueva = 0;

            if (!path.empty())
            {

                for (int p = 1; p < path.size(); p++)
                {
                    distanciaNueva += vertices[path[p]]->getTerritorio().getUnidadesDeEjercito_territorio();
                }

                if (distanciaNueva < distanciaMenor && vertices[path[0]]->getTerritorio().getUnidadesDeEjercito_territorio() > distanciaNueva)
                {

                    distanciaMenor = distanciaNueva;
                    cambio = true;
                    indiceDesde = indice1;
                    indiceHacia = indice2;
                }
            }
        }
    }

    if (cambio)
    {
        path = construirCamino(indiceDesde, indiceHacia);
        if (!path.empty())
        {
            imprimirCamino(path);
        }
    }
    else
    {
        cout << "Usted actualmente no cuenta con ningun territorio desde el cual la conquista mas barata sea rentable" << endl;
    }
}

void GrafoAdyacencia::inicializarFloydWarshall()
{
    for (int i = 0; i < nmbVertices; i++)
    {
        for (int j = 0; j < nmbVertices; j++)
        {
            if (i == j)
            {
                dist[i][j] = 0;
            }
            else if (aristas[i][j] != 0)
            {
                dist[i][j] = aristas[i][j];
            }

            if (dist[i][j] == numeric_limits<float>::max())
            {

                camino[i][j] = -1;
            }
            else
            {
                camino[i][j] = j;
            }
        }
    }
}

vector<int> GrafoAdyacencia::construirCamino(int u, int v)
{

    if (camino[u][v] == -1)
    {
        return {};
    }

    vector<int> secuencia = {u};
    while (u != v)
    {
        u = camino[u][v];
        secuencia.push_back(u);
    }
    return secuencia;
}

void GrafoAdyacencia::imprimirCamino(vector<int> &cam)
{
    int tam = 0;
    int n = cam.size();

    cout << "La conquista mas barata es avanzar sobre el territorio " << vertices[cam[n - 1]]->getTerritorio().getNombre() << " desde el territorio " << vertices[cam[0]]->getTerritorio().getNombre() << ". Para conquistar el territorio " << vertices[cam[n - 1]]->getTerritorio().getNombre() << ", debe atacar desde " << vertices[cam[0]]->getTerritorio().getNombre();

    tam += vertices[cam[n - 1]]->getTerritorio().getUnidadesDeEjercito_territorio();

    if (n == 2)
    {
        cout << ", no hace falta pasar por ningun otro territorio ";
    }
    else
    {
        for (int i = 1; i < n - 1; i++)
        {
            cout << vertices[cam[i]]->getTerritorio().getNombre();
            if (i < n - 2)
            {
                cout << ",";
            }
            tam += vertices[cam[i]]->getTerritorio().getUnidadesDeEjercito_territorio();
        }
    }

    cout << ". Debe conquistar " << tam << " unidades de ejercito." << endl;
}

void GrafoAdyacencia::Dijkstra(vertice *verticeOrigen, Tablero tab)
{
    int indiceOriginal = getIndice(verticeOrigen);

    vector<float> distancia(nmbVertices, numeric_limits<float>::max());
    vector<int> pariente(nmbVertices, -1);

    distancia[indiceOriginal] = 0;

    for (int i = 0; i < nmbVertices - 1; ++i)
    {
        int u = -1;
        for (int v = 0; v < nmbVertices; ++v)
        {
            if (!visitados[v] && (u == -1 || distancia[v] < distancia[u]))
            {
                u = v;
            }
        }

        if (u != -1)
        {
            visitados[u] = true;
        }

        for (int v = 0; v < nmbVertices; ++v)
        {
            if (!visitados[v] && aristas[v][u] > 0)
            {

                if (distancia[u] + aristas[v][u] < distancia[v])
                {
                    distancia[v] = distancia[u] + aristas[v][u];
                    pariente[v] = u;
                }
            }
        }
    }

    vertice *verticeMin = nullptr;
    float distanciaMinima = numeric_limits<float>::max();

    Jugador *jugadorActual = tab.getJugadoresTablero().front();

    bool territorioEncontrado = false;
    bool jugadorCorrecto = false;

    for (Territorio *ter : jugadorActual->getTerritorios_jugador())
    {
        int indiceVer = getIndicePorID(ter->getId());

        if (!vertices[indiceVer]->getVisitado() && distancia[indiceVer] < distanciaMinima && vertices[indiceVer]->getTerritorio().getUnidadesDeEjercito_territorio() > distancia[indiceVer])
        {
            distanciaMinima = distancia[indiceVer];
            verticeMin = vertices[indiceVer];
        }
    }

    if (verticeMin == nullptr)
    {
        cout << "El jugador " << jugadorActual->getNombre() << " no cuenta con ningun territorio desde el cual sea rentable conquistar " << verticeOrigen->getTerritorio().getNombre() << endl;
        return;
    }

    int costoTotal = 0;

    cout << "Para conquistar el territorio " << verticeOrigen->getTerritorio().getNombre() << ", debe atacar desde "
         << verticeMin->getTerritorio().getNombre();

    costoTotal += verticeOrigen->getTerritorio().getUnidadesDeEjercito_territorio();

    int indiceMin = getIndicePorID(verticeMin->getTerritorio().getId());

    if (pariente[indiceMin] != -1)
    {
        int actual = indiceMin;
        vector<Territorio> territoriosIntermedios;

        while (actual != indiceOriginal)
        {
            territoriosIntermedios.push_back(vertices[actual]->getTerritorio());

            actual = pariente[actual];
        }
        if (territoriosIntermedios.size() > 1)
        {
            cout << ", pasando por los territorios ";
        }
        else
        {
            cout << ", no hace falta pasar por ningun otro territorio";
        }

        // Imprimir territorios intermedios en orden inverso
        for (int i = 1; i < territoriosIntermedios.size(); ++i)
        {
            cout << territoriosIntermedios[i].getNombre();
            costoTotal += territoriosIntermedios[i].getUnidadesDeEjercito_territorio();

            if (i != territoriosIntermedios.size() - 1)
            {
                cout << ", ";
            }
        }
    }

    cout << ". Debe conquistar " << costoTotal << " unidades de ejercito." << endl;

    vaciar_visitados();

    return;
}

int GrafoAdyacencia::getIndicePorID(int id)
{
    int i = 0;
    while (i < nmbVertices)
    {
        if (vertices[i]->getTerritorio().getId() == id)
        {
            return i;
        }
        i++;
    }
    return -1;
}

vertice *GrafoAdyacencia::getVertices(int indice)
{
    if (indice >= 0 && indice < nmbVertices)
    {
        return vertices[indice];
    }
    return nullptr;
}

vector<vertice *> GrafoAdyacencia::getListaVertices() { return vertices; }

double GrafoAdyacencia::getDistanciaTotal()
{
    return distanciaTotal;
}