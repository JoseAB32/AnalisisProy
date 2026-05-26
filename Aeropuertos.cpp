#include "Aeropuertos.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

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

void cargarAeropuertos(const string &nombreArchivo, unordered_map<int, Aeropuerto> &aeropuertos, unordered_map<string, int> &indiceBusqueda, vector<vector<int>> &grafo) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de aeropuertos." << endl;
        return;
    }

    string linea;
    getline(archivo, linea);

    int idMaximo = -1;

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
        if (aeropuerto.id > idMaximo) {
            idMaximo = aeropuerto.id;
        }

        if (!aeropuerto.iata.empty()) {
            indiceBusqueda[convertirMinusculas(aeropuerto.iata)] = aeropuerto.id;
        }

        indiceBusqueda[convertirMinusculas(aeropuerto.nombre)] = aeropuerto.id;
    }

    if (idMaximo >= 0) {
        grafo.assign((idMaximo) + 1, vector<int>());
    }

    archivo.close();
}

void cargarRutas(const string &nombreArchivo, vector<vector<int>> &grafo, const unordered_map<int, Aeropuerto> &aeropuertos) {
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