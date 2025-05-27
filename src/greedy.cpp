/*
#include <cassert>
#include <greedy.h>
#include <iostream>
#include "mindiff.h"
#include "random.hpp"
#include <limits>
#include <algorithm>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

int GreedySearch::nodoMin(vector<tOption> &values, tSolution & sol,double ** matriz){
    double max_actual,min_actual,max_global,min_global;
    tOption dis_u;
    int nodo_min;
    max_global = 0;
    min_global = numeric_limits<double>::infinity();
    for(int i = 0; i < values.size();i++){
        dis_u = 0;
        min_actual = numeric_limits<double>::infinity();
        max_actual = 0;
        vector<int> dis_v(sol.size());

        for(int j = 0; j < sol.size();j++){
            if(values[i] < sol[j]){
                dis_u += matriz[values[i]][sol[j]];
            }
            else{
                dis_u += matriz[sol[j]][values[i]];
            }
        }
        
        for(int j = 0 ; j < sol.size();j++){
            for(int v : sol){
                if(sol[j] < v)
                    dis_v[j] += matriz[sol[j]][v];
                else
                    dis_v[j] += matriz[v][sol[j]];
            }
            if (sol[j] < i)
                dis_v[j] += matriz[sol[j]][i];
            else
                dis_v[j] += matriz[i][sol[j]];
        }

        max_actual = max(dis_u,*max_element(dis_v.begin(),dis_v.end()));
        min_actual = min(dis_u, *min_element(dis_v.begin(),dis_v.end()));

        if (max_actual - min_actual < max_global - min_global){
            max_global = max_actual;
            min_global = min_actual;
            nodo_min = values[i];
        }
    }
    return nodo_min;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 *//*
ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals ==1);
  Mindiff *realproblem = dynamic_cast<Mindiff *>(problem);
  auto size = problem->getSolutionSize();
  int u;
  double ** matriz = realproblem->getMatriz();
  vector<tOption> subValues;
  //Paso 1
  tSolution sol(problem->getProblemSize(),false);
  
  //Paso 2
  vector<tOption> values;
  for (int i = 0; i < size; i++) {
    values.push_back(i);
  }

  //Paso 3
  tDomain v = Random::get<uint8_t>(0,size-1);
  
  //Paso 4
  sol[v] = true;

  //Paso 5
  values.erase(values.begin() + v);

  //Paso 6
  while (sol.size() < size){
    //Paso 7
    subValues = values;
    //Paso 8
    u = nodoMin(subValues,sol,matriz);
    //Paso 9
    sol[u] = true;
    //Paso 10
    values.erase(values.begin()+u);
  }
  tFitness fitness = problem->fitness(sol);
  return ResultMH(sol, fitness, 1);
}
*/

#include "greedy.h"
#include "random.hpp"
#include "mindiff.h"
#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>

ResultMH GreedySearch::optimize(Problem *problem, int /*maxevals*/) {
    // 1) Tamaños del problema
    int n = problem->getProblemSize();    // número total de elementos
    int m = problem->getSolutionSize();   // cuántos escoger

    // 2) Representación de la solución: vector<bool> de tamaño n
    std::vector<bool> sol(n, false);

    // 3) Construir la lista de candidatos CL = {0,1,...,n-1}
    std::vector<int> CL(n);
    std::iota(CL.begin(), CL.end(), 0);

    // 4) Selección aleatoria inicial v0 ← SelectRandom(CL)
    int idx0 = Random::get<int>(0, CL.size() - 1);
    int v0   = CL[idx0];
    sol[v0]  = true;
    CL.erase(CL.begin() + idx0);

    // 5) Inicializar sumas de distancias de cada i∉S al conjunto S={v0}
    //    sumDist[i] = ∑_{v∈S} d(i,v)
    std::vector<double> sumDist(n, 0.0);
    for (int i = 0; i < n; ++i) {
        sumDist[i] = (i == v0 ? 0.0
                              : problem->getDistancia(i, v0));
    }

    // 6) Guardar la lista de seleccionados para actualizar fácilmente curMax/curMin
    std::vector<int> S;
    S.reserve(m);
    S.push_back(v0);

    // 7) inicializar curMax = curMin = sumDist[v0] = 0
    double curMax = 0.0, curMin = 0.0;

    // 8) Bucle constructivo: mientras |S| < m
    while ((int)S.size() < m) {
        int bestPos = -1;
        double bestG = std::numeric_limits<double>::infinity();

        // 8.1) Para cada candidato u en CL, calcular
        //      g(u) = max(curMax, sumDist[u]) - min(curMin, sumDist[u])
        for (int pos = 0; pos < (int)CL.size(); ++pos) {
            int u = CL[pos];
            double su   = sumDist[u];
            double gmax = std::max(curMax, su);
            double gmin = std::min(curMin, su);
            double gu   = gmax - gmin;

            if (gu < bestG) {
                bestG   = gu;
                bestPos = pos;
            }
        }

        // 8.2) Seleccionar el mejor u y añadirlo a S
        int u = CL[bestPos];
        sol[u]  = true;
        S.push_back(u);
        CL.erase(CL.begin() + bestPos);

        // 8.3) Actualizar sumDist[i] += d(i,u) para TODO i = 0..n-1
        for (int i = 0; i < n; ++i) {
            sumDist[i] += problem->getDistancia(i, u);
        }

        // 8.4) Recalcular curMax y curMin sobre los seleccionados S
        curMax = std::numeric_limits<double>::lowest();
        curMin = std::numeric_limits<double>::max();
        for (int v : S) {
            curMax = std::max(curMax, sumDist[v]);
            curMin = std::min(curMin, sumDist[v]);
        }
    }

    // 9) Evaluar la solución completa y devolverla
    tFitness fit = problem->fitness(sol);
    return ResultMH(sol, fit, /*una sola evaluación de fitness*/ 1);
}
