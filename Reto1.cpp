#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Aeropuerto {
    int id;
    string nombre;
    string ciudad;
    string pais;
    string iata;
    string icao;
    double latitud;
    double longitud;
};

string convertirMinusculas(string texto) {
    for (char &c : texto) {
        c = tolower(c);
    }

    return texto;
}

vector<string> separarPorTabulador(const string &linea) {
    vector<string> campos;
    string campo;
    stringstream ss(linea);

    while (getline(ss, campo, '\t')) {
        campos.push_back(campo);
    }

    return campos;
}

void cargarAeropuertos(const string &nombreArchivo, unordered_map<int, Aeropuerto> &aeropuertos, unordered_map<string, int> &indiceBusqueda) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de aeropuertos." << endl;
        return;
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        vector<string> campos = separarPorTabulador(linea);

        if (campos.size() < 8) {
            continue;
        }

        Aeropuerto aeropuerto;
        aeropuerto.id = stoi(campos[0]);
        aeropuerto.nombre = campos[1];
        aeropuerto.ciudad = campos[2];
        aeropuerto.pais = campos[3];
        aeropuerto.iata = campos[4];
        aeropuerto.icao = campos[5];
        aeropuerto.latitud = stod(campos[6]);
        aeropuerto.longitud = stod(campos[7]);

        aeropuertos[aeropuerto.id] = aeropuerto;

        if (!aeropuerto.iata.empty()) {
            indiceBusqueda[convertirMinusculas(aeropuerto.iata)] = aeropuerto.id;
        }

        indiceBusqueda[convertirMinusculas(aeropuerto.nombre)] = aeropuerto.id;
    }

    archivo.close();
}

void cargarRutas(const string &nombreArchivo, unordered_map<int, vector<int>> &grafo, const unordered_map<int, Aeropuerto> &aeropuertos) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de rutas." << endl;
        return;
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        vector<string> campos = separarPorTabulador(linea);

        if (campos.size() < 6) {
            continue;
        }

        int idOrigen = stoi(campos[3]);
        int idDestino = stoi(campos[5]);

        if (aeropuertos.find(idOrigen) != aeropuertos.end() && aeropuertos.find(idDestino) != aeropuertos.end()) {
            grafo[idOrigen].push_back(idDestino);
        }
    }

    archivo.close();
}

int contarAeropuertosAlcanzables(int idOrigen, const unordered_map<int, vector<int>> &grafo, int maxVuelos) {
    unordered_map<int, int> distancia;
    queue<int> cola;

    distancia[idOrigen] = 0;
    cola.push(idOrigen);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        if (distancia[actual] == maxVuelos) {
            continue;
        }

        auto it = grafo.find(actual);

        if (it == grafo.end()) {
            continue;
        }

        for (int vecino : it->second) {
            if (distancia.find(vecino) == distancia.end()) {
                distancia[vecino] = distancia[actual] + 1;
                cola.push(vecino);
            }
        }
    }

    return distancia.size() - 1;
}

void mostrarAeropuertosAlcanzables(int idOrigen, const unordered_map<int, vector<int>> &grafo, const unordered_map<int, Aeropuerto> &aeropuertos, int maxVuelos) {
    unordered_map<int, int> distancia;
    queue<int> cola;

    distancia[idOrigen] = 0;
    cola.push(idOrigen);

    cout << endl << "Aeropuertos alcanzables:" << endl;

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        if (distancia[actual] == maxVuelos) {
            continue;
        }

        auto itGrafo = grafo.find(actual);

        if (itGrafo == grafo.end()) {
            continue;
        }

        for (int vecino : itGrafo->second) {
            if (distancia.find(vecino) == distancia.end()) {
                distancia[vecino] = distancia[actual] + 1;
                cola.push(vecino);

                auto itAeropuerto = aeropuertos.find(vecino);

                if (itAeropuerto != aeropuertos.end()) {
                    const Aeropuerto &a = itAeropuerto->second;
                    cout << "- " << a.nombre << " (" << a.iata << ") | " << a.ciudad << ", " << a.pais << " | vuelos: " << distancia[vecino] << endl;
                }
            }
        }
    }
}

int main() {
    unordered_map<int, Aeropuerto> aeropuertos;
    unordered_map<string, int> indiceBusqueda;
    unordered_map<int, vector<int>> grafo;

    cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda);
    cargarRutas("rutas_limpio.txt", grafo, aeropuertos);

    cout << "Aeropuertos cargados: " << aeropuertos.size() << endl;
    cout << "Nodos con rutas de salida: " << grafo.size() << endl;

    string entrada;
    cout << endl << "Ingrese nombre o codigo IATA del aeropuerto de origen: ";
    getline(cin, entrada);

    string clave = convertirMinusculas(entrada);

    if (indiceBusqueda.find(clave) == indiceBusqueda.end()) {
        cout << "No se encontro el aeropuerto ingresado." << endl;
        return 0;
    }

    int idOrigen = indiceBusqueda[clave];
    const int MAX_VUELOS = 4;

    int total = contarAeropuertosAlcanzables(idOrigen, grafo, MAX_VUELOS);

    cout << endl;
    cout << "Aeropuerto origen: " << aeropuertos[idOrigen].nombre << " (" << aeropuertos[idOrigen].iata << ")" << endl;
    cout << "Cantidad de aeropuertos alcanzables con maximo 3 escalas: " << total << endl;

    mostrarAeropuertosAlcanzables(idOrigen, grafo, aeropuertos, MAX_VUELOS);

    return 0;
}