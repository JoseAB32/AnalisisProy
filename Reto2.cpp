#include <bits/stdc++.h>
#include "Aeropuertos.h"
#define form(i,s,e) for(int i = s; i < e; i++)
using namespace std;

vector<bool> visitados;
vector<int> st, scc;

void dfs(int u, unordered_map<int, set<int>> &grafo, int numero_componente)
{
  scc[u] = numero_componente;

  visitados[u] = true;
  auto itGrafo = grafo.find(u);
  for(auto v: itGrafo->second)
    if(visitados[v] == false)
      dfs(v,grafo,numero_componente);
      
  st.push_back(u);
}


void dfs1(unordered_map<int, set<int>> &grafo,int maxId )
{
  visitados.assign(maxId + 1, false);
  scc.assign(maxId + 1, 0);
  for(auto par: grafo)
    if(visitados[par.first] == false)
      dfs(par.first, grafo, 0);
}

unordered_map<int, set<int>> invertir_grafo(unordered_map<int, set<int>> &grafo, int maxId)
{
  unordered_map<int, set<int>> grafo_invertido;
  for(auto par: grafo)
  {
    grafo_invertido[par.first];
    for(auto v: par.second)
      grafo_invertido[v].insert(par.first);
  }
  return grafo_invertido;
}

int maxSCC = -1; vector<int> scc_tamanio;
int dfs2(unordered_map<int, set<int>> &grafo_invertido, int maxId)
{
  reverse(st.begin(), st.end());

  scc_tamanio.assign(maxId + 1, 0);
  visitados.assign(maxId + 1, false);
  int maxi = st.size(), cont = 0;
  form(i,0,maxi)
    if(visitados[st[i]] == false )
    {
      int anterior = st.size();
      dfs(st[i],grafo_invertido,cont);
      scc_tamanio[cont] = ((int)st.size()) - anterior;
      cont++;
      maxSCC = max(maxSCC,((int)st.size()) - anterior);
    }
  return cont;
}

int maxAislado = -1;
int contarGruposAislados(unordered_map<int, set<int>> &grafo)
{
  unordered_map<int, bool> gruposAislados;
  for(auto par: grafo)
  {
    auto [i , vec] = par;
    int grupo = scc[i];
    if(gruposAislados.find(grupo) == gruposAislados.end())
      gruposAislados[grupo] = true;
    for(auto e: vec)
      if(scc[e] != grupo)
        gruposAislados[grupo] = gruposAislados[scc[e]] = false;
  }
  int cont = 0;
  for(auto par: gruposAislados)
    if(par.second)
    {
      maxAislado = max(maxAislado, scc_tamanio[par.first]);
      cont++;
    }
  return cont;
}

int main()
{
  unordered_map<int, Aeropuerto> aeropuertos;
  unordered_map<string, int> indiceBusqueda;
  unordered_map<int, set<int>> grafo;

  int maxId = cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
  cargarRutas("rutas_limpio.txt", grafo, aeropuertos);

  
  dfs1(grafo, maxId);
  unordered_map<int, set<int>> grafo_invertido = invertir_grafo(grafo, maxId);
  
  cout <<"Componentes fuertemente conexas: " <<dfs2(grafo_invertido, maxId) << endl;
  cout <<"Componente fuertemente conexa mas grande: " << maxSCC << endl;


  cout<<"Grupos Aislados: " << contarGruposAislados(grafo) << endl;
  cout<<"Grupo Aislado mas grande: " << maxAislado << endl;
  return 0;
}