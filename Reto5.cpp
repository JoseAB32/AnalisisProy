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

unordered_map<int, vector<pair<int, double>>> construirGrafoAerolinea(unordered_map<int, set<int>> &grafo_Aerolinea, unordered_map<int, Aeropuerto> &aeropuertos) {
    unordered_map<int, vector<pair<int, double>>> grafo_Aerolinea_conPesos;

    for (auto par : grafo_Aerolinea) {
        int idOrigen = par.first;
        set<int> destinos = par.second;

        if (destinos.empty()) {
            continue;
        }

        Aeropuerto origen = aeropuertos[idOrigen];

        for (int idDestino : destinos) {
            Aeropuerto destino = aeropuertos[idDestino];
            double distancia = calcularDistanciaGeo(origen.latitud, origen.longitud, destino.latitud, destino.longitud);

            grafo_Aerolinea_conPesos[idOrigen].push_back({idDestino, distancia});
        }
    }

    return grafo_Aerolinea_conPesos;
}

pair<double, vector<int>> dijkstraAerolinea(int idOrigen, int idDestino, unordered_map<int, vector<pair<int, double>>> &grafo_Aerolinea) {
    unordered_map<int, double> dist;
    unordered_map<int, int> padre;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> cola;

    dist[idOrigen] = 0.0;
    padre[idOrigen] = -1;
    cola.push({0.0, idOrigen});

    while (!cola.empty()) {
        double distanciaActual = cola.top().first;
        int actual = cola.top().second;
        cola.pop();

        if (distanciaActual > dist[actual]) {
            continue;
        }

        if (actual == idDestino) {
            break;
        }

        auto it = grafo_Aerolinea.find(actual);

        if (it == grafo_Aerolinea.end()) {
            continue;
        }

        for (auto arista : it->second) {
            int vecino = arista.first;
            double peso = arista.second;
            double nuevaDistancia = distanciaActual + peso;

            if (dist.find(vecino) == dist.end() || nuevaDistancia < dist[vecino]) {
                dist[vecino] = nuevaDistancia;
                padre[vecino] = actual;
                cola.push({nuevaDistancia, vecino});
            }
        }
    }

    vector<int> camino;

    if (dist.find(idDestino) == dist.end()) {
        return {-1.0, camino};
    }

    int actual = idDestino;

    while (actual != -1) {
        camino.push_back(actual);
        actual = padre[actual];
    }

    reverse(camino.begin(), camino.end());

    return {dist[idDestino], camino};
}

int main() {
    unordered_map<int, Aeropuerto> aeropuertos;
    unordered_map<string, int> indiceBusqueda;
    unordered_map<int, set<int>> grafoBase;
    unordered_map<int, set<int>> grafo_Aerolinea;

    cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafoBase);

    string aerolinea;
    string entradaOrigen;
    string entradaDestino;

    cout << "Ingrese codigo de aerolinea: ";
    getline(cin, aerolinea);

    cout << "Ingrese aeropuerto origen (nombre o IATA): ";
    getline(cin, entradaOrigen);

    cout << "Ingrese aeropuerto destino (nombre o IATA): ";
    getline(cin, entradaDestino);

    string claveOrigen = convertirMinusculas(entradaOrigen);
    string claveDestino = convertirMinusculas(entradaDestino);

    if (indiceBusqueda.find(claveOrigen) == indiceBusqueda.end()) {
        cout << "No se encontro el aeropuerto origen." << endl;
        return 0;
    }

    if (indiceBusqueda.find(claveDestino) == indiceBusqueda.end()) {
        cout << "No se encontro el aeropuerto destino." << endl;
        return 0;
    }

    int idOrigen = indiceBusqueda[claveOrigen];
    int idDestino = indiceBusqueda[claveDestino];

    int aristas = cargarRutasPorAerolinea("rutas_limpio.txt", grafo_Aerolinea, aeropuertos, aerolinea);

    if (aristas == 0) {
        cout << "No se encontraron rutas para la aerolinea ingresada." << endl;
        return 0;
    }

    unordered_map<int, vector<pair<int, double>>> grafo_Aerolinea_conPesos = construirGrafoAerolinea(grafo_Aerolinea, aeropuertos);

    pair<double, vector<int>> resultado = dijkstraAerolinea(idOrigen, idDestino, grafo_Aerolinea_conPesos);

    if (resultado.first < 0) {
        cout << "No existe camino entre los aeropuertos usando exclusivamente la aerolinea " << aerolinea << "." << endl;
        return 0;
    }

    cout << fixed << setprecision(2);
    cout << endl;
    cout << "---- RETO 5: EL PASAJERO LEAL ----" << endl;
    cout << "Aerolinea utilizada: " << aerolinea << endl;
    cout << "Nombre Aeropuerto Origen:  " << aeropuertos[idOrigen].nombre << "  Codigo IATA Origen:  " << aeropuertos[idOrigen].iata << endl;
    cout << "Nombre Aeropuerto Destino: " << aeropuertos[idDestino].nombre << "  Codigo IATA Destino:  " << aeropuertos[idDestino].iata << endl;
    cout << "Distancia minima encontrada: " << resultado.first << " km" << endl;
    cout << endl;
    cout << "Camino encontrado:" << endl;

    for (int id : resultado.second) {
        cout << "- " << aeropuertos[id].nombre << " (" << aeropuertos[id].iata << ")" << endl;
    }

    return 0;
}