#include <bits/stdc++.h>
#include "Aeropuertos.h"
#define form(i,s,e) for(int i = s; i < e; i++)
using namespace std;


int main()
{
  unordered_map<int, Aeropuerto> aeropuertos;
  unordered_map<string, int> indiceBusqueda;
  unordered_map<int, set<int>> grafo;

  int maxId = cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
  int aristas =  cargarRutasNoDirido("rutas_limpio.txt", grafo, aeropuertos);
  cout << "Aristas Bidireccionales: " << aristas << endl;
  
  return 0;
}