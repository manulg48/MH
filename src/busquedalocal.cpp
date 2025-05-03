#include <cassert>
#include "mindiff.h"
#include "busquedalocal.h"
#include "random.hpp"
#include <algorithm>
#include <iostream>
using namespace std;
bool BusquedaLocal::verifica(const vector<tOption>& sel, tOption i, tOption j, int m) {
    if (find(sel.begin(), sel.end(), j) != sel.end())  // j ya está en sel → no válido
        return false;

    auto it = find(sel.begin(), sel.end(), i);
    if (it == sel.end())  // i no está en sel → no válido
        return false;

    vector<tOption> nuevaSel = sel;
    *find(nuevaSel.begin(), nuevaSel.end(), i) = j;  // simular el cambio

    sort(nuevaSel.begin(), nuevaSel.end());
    if (adjacent_find(nuevaSel.begin(), nuevaSel.end()) != nuevaSel.end())
        return false;

    return nuevaSel.size() == m;
}


void BusquedaLocal::actualizarSolucion(vector<tDomain> &sol,vector<tOption> & sel){
    for(int i = 0; i < sol.size(); i++){
        sol[i] = false;
    }
    for(int i = 0; i < sel.size();i++){
        sol[sel[i]] = true;
    }
}

bool BusquedaLocal::calcularCoste(vector<tOption> &sel, tOption i, tOption j, Mindiff *problema, tFitness costeActual, tSolution &sol) {
    int m = sel.size();
    double **matriz = problema->getMatriz();
    
    // Paso 1: crear una copia de sel con el cambio simulado i → j
    vector<tOption> sel_nuevo = sel;
    sel_nuevo[i] = j;

    // Paso 2: calcular nuevas contribuciones para cada nodo del nuevo conjunto
    vector<double> contribuciones(m, 0.0);

    for (int u = 0; u < m; ++u) {
        for (int v = 0; v < m; ++v) {
            if (u == v) continue;
            int a = sel_nuevo[u];
            int b = sel_nuevo[v];
            // Acceso correcto a matriz triangular
            if (a < b)
                contribuciones[u] += matriz[a][b];
            else
                contribuciones[u] += matriz[b][a];
        }
    }

    // Paso 3: calcular nuevo min y max de ∆(v)
    double min_c = std::numeric_limits<double>::max();
    double max_c = std::numeric_limits<double>::lowest();
    for (int k = 0; k < m; ++k) {
        if (contribuciones[k] < min_c) min_c = contribuciones[k];
        if (contribuciones[k] > max_c) max_c = contribuciones[k];
    }

    double nuevoCoste = max_c - min_c;

    // Paso 4: comparar contra el coste actual (con tolerancia)
    if (nuevoCoste + 1e-9 < costeActual) {
        // Aceptamos el cambio
        problema->setCosteActual(nuevoCoste);
        problema->setDistancia(contribuciones.data(), m);
        problema->setRangoDistancia({min_c, max_c});

        // Actualizar solución booleana
        int nodo_i = sel[i];
        sol[nodo_i] = false;
        sol[j] = true;

        return true;
    }

    return false;
}

ResultMH BusquedaLocal::optimize(Problem *problem, int maxevals){
    assert(maxevals == 100000);
    Mindiff *realproblem = dynamic_cast<Mindiff *>(problem);
    auto m = problem->getSolutionSize();
    auto n = problem->getProblemSize();
    double ** matriz = realproblem->getMatriz();
    tOption j;
    tSolution sol = problem->createSolution();
    tFitness fitness = problem->fitness(sol);
    vector<tOption> values;
    vector<tOption> valuesOriginal;
    vector<tOption> vecinos;
    int evaluaciones = 0;
    bool coste;
    for(int i = 0; i < n; i++){
        if (sol[i])
            values.push_back(i);
    }
    valuesOriginal = values;
    
    vector<tOption> valuesSeleccionados;
    int l;
    for(int i = 0; i < m && evaluaciones < maxevals; i++){
        vecinos.clear();
        for(int l = 0; l < n; l++){
            vecinos.push_back(l);
        }
    
        for(int z = 0; z < m;z++){
            vecinos.erase(remove(vecinos.begin(),vecinos.end(),values[z]),vecinos.end());
        }
        do{
            l = Random::get<tOption>(0,values.size()-1);
        }while(find(valuesSeleccionados.begin(),valuesSeleccionados.end(),valuesOriginal[l]) != valuesSeleccionados.end());
        valuesSeleccionados.push_back(l);
        do{
            do{
                j = Random::get<tOption>(0,n-1);
                if (!verifica(values, values[l], j, m)) {
                }
                
            }while((!verifica(values, values[l], j, m)) || (find(vecinos.begin(), vecinos.end(), j) == vecinos.end()));
            coste = calcularCoste(values, l, j, realproblem, fitness, sol);
            if (coste) {
                values[l] = j;
                actualizarSolucion(sol, values);
                fitness = realproblem->getCosteActual();
                vecinos.erase(remove(vecinos.begin(),vecinos.end(), j),vecinos.end());
                break;
            }
            vecinos.erase(remove(vecinos.begin(),vecinos.end(),j),vecinos.end());
            evaluaciones++;
        }while (!coste && evaluaciones < maxevals && vecinos.size() > 0);
    }
    return ResultMH(sol, fitness,evaluaciones);
}

