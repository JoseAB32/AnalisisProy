#include <bits/stdc++.h>
#include "Aeropuertos.h"
#define form(i,s,e) for(int i = s; i < e; i++)
using namespace std;

// 1. Fórmula Geodésica (Proporcionada en el PDF del proyecto)
double calcularDistanciaGeo(double lat1, double lon1, double lat2, double lon2) {
  const double R = 6371.0; 
  const double PI = 3.141592653589793;
  double phi1 = lat1 * PI / 180.0;
  double phi2 = lat2 * PI / 180.0;
  double dPhi = (lat2 - lat1) * PI / 180.0;
  double dLam = (lon2 - lon1) * PI / 180.0;
  double a = sin(dPhi / 2) * sin(dPhi / 2) + 
              cos(phi1) * cos(phi2) * sin(dLam / 2) * sin(dLam / 2);
  return R * 2 * atan2(sqrt(a), sqrt(1 - a));
}

struct Arista {
    int u, v;
    double peso;
    bool operator<(const Arista& otra) const {
      return peso < otra.peso;
    }
};

struct DSU {
  vector<int> parent;
  
  DSU(int n) {
    parent.resize(n + 1);
    iota(parent.begin(), parent.end(), 0);
  }
  
  int find_set(int v) {
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]);
  }
  
  bool union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      parent[b] = a; 
      return true;
    }
    return false;
  }
};

int main()
{
  unordered_map<int, Aeropuerto> aeropuertos;
  unordered_map<string, int> indiceBusqueda;
  unordered_map<int, set<int>> grafo;

  int maxId = cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
  int aristas = cargarRutasNoDirido("rutas_limpio.txt", grafo, aeropuertos);
  
  set<string> sudamerica = {
    "Argentina", "Bolivia", "Brazil", "Chile", "Colombia", 
    "Ecuador", "Guyana", "Paraguay", "Peru", "Suriname", 
    "Uruguay", "Venezuela", "French Guiana", "Falkland Islands"
  };

  vector<Arista> rutasSudamerica;

  for(auto const& [u, destinos] : grafo) {
    for(int v : destinos) {
      if(u < v) {
        if(sudamerica.count(aeropuertos[u].pais) && sudamerica.count(aeropuertos[v].pais)) {
            
          double peso = calcularDistanciaGeo(
            aeropuertos[u].latitud, aeropuertos[u].longitud,
            aeropuertos[v].latitud, aeropuertos[v].longitud
          );
          
          rutasSudamerica.push_back({u, v, peso});
        }
      }
    }
  }

  sort(rutasSudamerica.begin(), rutasSudamerica.end());
  
  DSU dsu(maxId);
  double costoTotalMST = 0.0;
  int aristasUsadas = 0;

  for(const Arista& ruta : rutasSudamerica) {
    if(dsu.union_sets(ruta.u, ruta.v)) {
      costoTotalMST += ruta.peso;
      aristasUsadas++;
    }
  }

  cout << "Aristas Bidireccionales totales: " << aristas << endl;
  cout << "Aristas analizadas en Sudamerica: " << rutasSudamerica.size() << endl;
  cout << "Rutas seleccionadas para la Red Minima (MST): " << aristasUsadas << endl;
  cout << "Kilometraje total de la Red Minima: " << fixed << setprecision(2) << costoTotalMST << " km" << endl;

  return 0;
}