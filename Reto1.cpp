#include <iostream>
#include <queue>

#include "Aeropuertos.h"

using namespace std;

int mostrarYContarAeropuertosAlcanzables(int idOrigen, int idMaximo, const unordered_map<int, set<int>> &grafo, const unordered_map<int, Aeropuerto> &aeropuertos, int maxVuelos) {
    vector<int> dist(idMaximo + 1, -1);
    queue<int> cola;


    dist[idOrigen] = 0;
    cola.push(idOrigen);

    cout << endl << "Aeropuertos alcanzables:" << endl;

    int total = 0;

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        if (dist[actual] == maxVuelos) {
            continue;
        }
        
        auto itGrafo = grafo.find(actual);


        for (int vecino : itGrafo->second) {
            if (dist[vecino] == -1) {
                dist[vecino] = dist[actual] + 1;
                cola.push(vecino);

                total++;

                auto itAeropuerto = aeropuertos.find(vecino);

                const Aeropuerto &a = itAeropuerto->second;
                cout << "- " << a.nombre << " (" << a.iata << ") | " << a.ciudad << ", " << a.pais << " | vuelos: " << dist[vecino] << endl;
            }
        }
    }

    return total;
}

int main() {
    unordered_map<int, Aeropuerto> aeropuertos;
    unordered_map<string, int> indiceBusqueda;
    unordered_map<int, set<int>> grafo;

    int idMaximo = cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
    int aristas = cargarRutas("rutas_limpio.txt", grafo, aeropuertos);

    cout << "Aeropuertos cargados(Nodos): " << aeropuertos.size() << endl;
    cout << "Aristas Unidireccionales: " << aristas << endl;

    string entrada;
    cout << endl << "Ingrese nombre o codigo IATA del aeropuerto de origen: ";
    getline(cin, entrada);

    string clave = convertirMinusculas(entrada);

    if (indiceBusqueda.find(clave) == indiceBusqueda.end()) {
        cout << "No se encontro el aeropuerto ingresado." << endl;
        return 0;
    }

    int idOrigen = indiceBusqueda[clave];
    const int MAX_VUELOS = 5;

    int total = mostrarYContarAeropuertosAlcanzables(idOrigen, idMaximo, grafo, aeropuertos, MAX_VUELOS);

    cout << endl;
    cout << "Aeropuerto origen: " << aeropuertos[idOrigen].nombre << " (" << aeropuertos[idOrigen].iata << ")" << endl;
    cout << "Cantidad de aeropuertos alcanzables con maximo 3 escalas: " << total << endl;

    return 0;
}