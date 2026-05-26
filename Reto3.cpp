#include <bits/stdc++.h>
#include "Aeropuertos.h"

using namespace std;

double calcularDistanciaGeo(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;
    const double PI = 3.141592653589793;

    double phi1 = lat1 * PI / 180.0;
    double phi2 = lat2 * PI / 180.0;
    double dPhi = (lat2 - lat1) * PI / 180.0;
    double dLam = (lon2 - lon1) * PI / 180.0;

    double a = sin(dPhi / 2) * sin(dPhi / 2) + cos(phi1) * cos(phi2) * sin(dLam / 2) * sin(dLam / 2);

    return R * 2 * atan2(sqrt(a), sqrt(1 - a));
}

unordered_map<int, vector<pair<int, double>>> construirGrafoConPesos(unordered_map<int, set<int>> &grafo, unordered_map<int, Aeropuerto> &aeropuertos) {
    unordered_map<int, vector<pair<int, double>>> grafo_conPesos;

    for (auto par : grafo) {
        int idOrigen = par.first;
        set<int> destinos = par.second;

        if (destinos.empty()) {
            continue;
        }

        Aeropuerto origen = aeropuertos[idOrigen];

        for (int idDestino : destinos) {
            Aeropuerto destino = aeropuertos[idDestino];
            double distancia = calcularDistanciaGeo(origen.latitud, origen.longitud, destino.latitud, destino.longitud);

            grafo_conPesos[idOrigen].push_back({idDestino, distancia});
        }
    }

    return grafo_conPesos;
}

pair<int, double> dijkstraMayorDistancia(int idOrigen, unordered_map<int, vector<pair<int, double>>> &grafo_conPesos) {
    unordered_map<int, double> dist;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> cola;

    dist[idOrigen] = 0.0;
    cola.push({0.0, idOrigen});

    int destinoMasLejano = idOrigen;
    double mayorDistancia = 0.0;

    while (!cola.empty()) {
        double distanciaActual = cola.top().first;
        int actual = cola.top().second;
        cola.pop();

        if (distanciaActual > dist[actual]) {
            continue;
        }

        if (distanciaActual > mayorDistancia) {
            mayorDistancia = distanciaActual;
            destinoMasLejano = actual;
        }

        auto it = grafo_conPesos.find(actual);

        if (it == grafo_conPesos.end()) {
            continue;
        }

        for (auto arista : it->second) {
            int vecino = arista.first;
            double peso = arista.second;
            double nuevaDistancia = distanciaActual + peso;

            if (dist.find(vecino) == dist.end() || nuevaDistancia < dist[vecino]) {
                dist[vecino] = nuevaDistancia;
                cola.push({nuevaDistancia, vecino});
            }
        }
    }

    return {destinoMasLejano, mayorDistancia};
}

pair<double, pair<int, int>> calcularDiametro(unordered_map<int, set<int>> &grafo, unordered_map<int, Aeropuerto> &aeropuertos) {
    unordered_map<int, vector<pair<int, double>>> grafo_conPesos = construirGrafoConPesos(grafo, aeropuertos);

    double mejorDistancia = -1.0;
    int mejorOrigen = -1;
    int mejorDestino = -1;

    for (auto par : grafo_conPesos) {
        int idOrigen = par.first;

        if (par.second.empty()) {
            continue;
        }

        pair<int, double> resultado = dijkstraMayorDistancia(idOrigen, grafo_conPesos);
        int idDestino = resultado.first;
        double distancia = resultado.second;

        if (distancia > mejorDistancia) {
            mejorDistancia = distancia;
            mejorOrigen = idOrigen;
            mejorDestino = idDestino;
        }
    }

    return {mejorDistancia, {mejorOrigen, mejorDestino}};
}

int main() {
    unordered_map<int, Aeropuerto> aeropuertos;
    unordered_map<string, int> indiceBusqueda;
    unordered_map<int, set<int>> grafo;

    cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
    int aristas = cargarRutas("rutas_limpio.txt", grafo, aeropuertos);

    cout << "Aeropuertos cargados(Nodos): " << aeropuertos.size() << endl;
    cout << "Aristas Unidireccionales: " << aristas << endl;

    pair<double, pair<int, int>> resultado = calcularDiametro(grafo, aeropuertos);

    double distancia = resultado.first;
    int idOrigen = resultado.second.first;
    int idDestino = resultado.second.second;

    Aeropuerto origen = aeropuertos[idOrigen];
    Aeropuerto destino = aeropuertos[idDestino];

    cout << fixed << setprecision(2);
    cout << endl;
    cout << "---- MAXIMO CAMINO MINIMO(DIJKSTRA) ----" << endl;
    cout << "Nombre Aeropuerto Origen:  " << origen.nombre << "  Codigo IATA Origen:  " << origen.iata << endl;
    cout << "Nombre Aeropuerto Destino: " << destino.nombre << "  Codigo IATA Destino:  " << destino.iata << endl;
    cout << "Distancia minima mas larga encontrada: " << distancia << " km" << endl;

    return 0;
}