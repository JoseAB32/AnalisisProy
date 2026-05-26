#include <bits/stdc++.h>
#include "Aeropuertos.h"
#define form(i,s,e) for(int i = s; i < e; i++)
using namespace std;

vector<bool> visitados;
vector<int> st, scc;
vector<vector<int>> nodos_componente;

void dfs(int u, unordered_map<int, vector<int>> &grafo, int numero_componente)
{
  nodos_componente[numero_componente].push_back(u);
  scc[u] = numero_componente;

  visitados[u] = true;
  auto itGrafo = grafo.find(u);
  for(auto v: itGrafo->second)
    if(visitados[v] == false)
      dfs(v,grafo,numero_componente);
      
  st.push_back(u);
}


void dfs1(unordered_map<int, vector<int>> &grafo,int maxId )
{
  visitados.assign(maxId + 1, false);
  scc.assign(maxId + 1, 0);
  nodos_componente.assign(maxId + 1, vector<int> ());
  for(auto par: grafo)
    if(visitados[par.first] == false)
      dfs(par.first, grafo, 0);
}

unordered_map<int, vector<int>> invertir_grafo(unordered_map<int, vector<int>> &grafo, int maxId)
{
  unordered_map<int, vector<int>> grafo_invertido;
  for(auto par: grafo)
  {
    grafo_invertido[par.first];
    for(auto v: par.second)
      grafo_invertido[v].push_back(par.first);
  }
  return grafo_invertido;
}

int dfs2(unordered_map<int, vector<int>> &grafo_invertido, int maxId)
{
  reverse(st.begin(), st.end());
  visitados.assign(maxId + 1, false);
  nodos_componente.assign(maxId + 1, vector<int> ());
  int max = st.size(), cont = 0;
  form(i,0,max)
    if(visitados[st[i]] == false )
      dfs(st[i],grafo_invertido,cont), cont++;
  return cont;
}

bool contarGruposAislados(int u, unordered_map<int, vector<int>> &grafo)
{
  visitados[u] = true;
  int componente_actual = scc[u];
  auto itGrafo = grafo.find(u);

  for(auto v: itGrafo->second)
  {
    if(visitados[v] == false)
      if(!contarGruposAislados(v,grafo))
        return false;
    if(componente_actual != scc[v])
      return false;
  }
  return true;
}

int main()
{
  unordered_map<int, Aeropuerto> aeropuertos;
  unordered_map<string, int> indiceBusqueda;
  unordered_map<int, vector<int>> grafo;

  int maxId = cargarAeropuertos("aeropuertos_limpio.txt", aeropuertos, indiceBusqueda, grafo);
  cargarRutas("rutas_limpio.txt", grafo, aeropuertos);

  
  dfs1(grafo, maxId);
  unordered_map<int, vector<int>> grafo_invertido = invertir_grafo(grafo, maxId);
  
  cout <<"Componentes fuertemente conexas: " <<dfs2(grafo_invertido, maxId) << endl;

  visitados.assign(maxId + 1, false);
  int cont = 0, maxAislado = -1, maxSCC = -1;
  form(i,0,nodos_componente.size())
    form(j, 0, nodos_componente[i].size())
    {
      maxSCC = max(maxSCC, (int)nodos_componente[i].size());
      if(visitados[nodos_componente[i][j]] == false and 
       contarGruposAislados(nodos_componente[i][j],grafo))
      {
        maxAislado = max(maxAislado, (int)nodos_componente[i].size());
        cont++;
      }
      else
        break;
    }

  
  cout <<"Componente fuertemente conexa mas grande: " << maxSCC << endl;

  cout<<"Grupos Aislados: " << cont << endl;
  cout<<"Grupo Aislado mas grande: " << maxAislado << endl;
  return 0;
}