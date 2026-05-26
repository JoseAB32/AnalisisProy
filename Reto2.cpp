#include <bits/stdc++.h>
#include "Aeropuertos.h"
#define form(i,s,e) for(int i = s; i < e; i++)
using namespace std;

vector<bool> vis;
vector<int> st, num_componente;
vector<vector<int>> componente;
void dfs(int u, unordered_map<int, vector<int>> &grafo, int numero_componente)
{
  componente[numero_componente].push_back(u);
  num_componente[u] = numero_componente;

  vis[u] = true;
  auto itGrafo = grafo.find(u);

  for(auto v: itGrafo->second)
    if(vis[v] == false)
      dfs(v,grafo,numero_componente);
      
  st.push_back(u);
}

bool contarGruposAislados(int u, unordered_map<int, vector<int>> &grafo)
{
  int componente_actual = num_componente[u];
  auto itGrafo = grafo.find(u);
  

  for(auto v: itGrafo->second)
      if(!contarGruposAislados(v,grafo) or componente_actual != num_componente[v] )
        return false;
  return true;
}


int main()
{
  unordered_map<int, Aeropuerto> aeropuertos;
  unordered_map<string, int> indiceBusqueda;
  unordered_map<int, vector<int>> grafo, grafo_invertido;

  cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
  cargarRutas("rutas_limpio.txt", grafo, aeropuertos);

  int maxId = 0;
  for (const auto &par : aeropuertos) {
    if (par.first > maxId) {
      maxId = par.first;
    }
  }

  vis.resize(maxId + 1, false);
  num_componente.resize(maxId + 1, 0);
  componente.resize(maxId + 1);
  form(i,0,maxId + 1)
    if(vis[i] == false && grafo.find(i) != grafo.end())
      dfs(i,grafo,0);


  form(i,0,maxId + 1)
    if (grafo.find(i) != grafo.end())
      for(auto v: grafo[i])
        grafo_invertido[v].push_back(i);
    
  componente.assign(maxId + 1,vector<int>());
  reverse(st.begin(), st.end());
  vis.assign(maxId + 1, false);
  int max = st.size(), cont = 0;
  form(i,0,max)
    if(vis[st[i]] == false )
      dfs(st[i],grafo_invertido,cont), cont++;

  cout << cont << endl;

  cont = 0;
  form(i,0,componente.size())
    form(j, 0, componente[i].size())
      if(grafo.find(componente[i][j]) != grafo.end() and vis[componente[i][j]] == false and contarGruposAislados(componente[i][j],grafo))
        cont++;

  cout << cont << endl;
  return 0;
}