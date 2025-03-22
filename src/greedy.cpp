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
 */
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