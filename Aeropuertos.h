#ifndef AEROPUERTOS_H
#define AEROPUERTOS_H

#include <bits/stdc++.h>

struct Aeropuerto {
    int id;
    std::string nombre;
    std::string ciudad;
    std::string pais;
    std::string iata;
    std::string icao;
    double latitud;
    double longitud;
};

std::string convertirMinusculas(std::string texto);
std::vector<std::string> separarPorTabulador(const std::string &linea);
int cargarAeropuertos(const std::string &nombreArchivo, std::unordered_map<int, Aeropuerto> &aeropuertos, std::unordered_map<std::string, int> &indiceBusqueda, std::unordered_map<int, std::set<int>> &grafo);
int cargarRutas(const std::string &nombreArchivo, std::unordered_map<int, std::set<int>> &grafo, const std::unordered_map<int, Aeropuerto> &aeropuertos);

#endif