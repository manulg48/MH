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
    Mindiff *realproblem = dynamic_cast<Mindiff *>(problem);
    auto m = problem->getSolutionSize();
    auto n = problem->getProblemSize();
    double **matriz = realproblem->getMatriz();
    tSolution sol = problem->createSolution();
    tFitness costeActual = problem->fitness(sol);
    int evaluaciones = 0;

    vector<tOption> seleccionados;
    for (int i = 0; i < n; ++i) {
        if (sol[i]) seleccionados.push_back(i);
    }

    bool mejora = true;

    while (mejora && evaluaciones < maxevals) {
        mejora = false;

        vector<pair<tOption, tOption>> vecinos;
        for (size_t i = 0; i < seleccionados.size(); ++i) {
            for (tOption j = 0; j < n; ++j) {
                if (!sol[j]) {
                    vecinos.emplace_back(i, j);
                }
            }
        }

        shuffle(vecinos.begin(), vecinos.end(), Random::engine());

        for (auto [i, j] : vecinos) {
            if (evaluaciones >= maxevals) break;
            evaluaciones++;

            if (verifica(seleccionados, seleccionados[i], j, m)) {
                if (calcularCoste(seleccionados, i, j, realproblem, costeActual, sol)) {
                    seleccionados[i] = j;
                    costeActual = problem->fitness(sol);
                    mejora = true;
                    break; // Primera mejora
                }
            }
        }
    }

    return ResultMH(sol, costeActual, evaluaciones);
}


ResultMH BusquedaLocal::optimize(Problem *problem, const tSolution &solution, tFitness fitness, int maxevals) {
    Mindiff *realproblem = dynamic_cast<Mindiff *>(problem);
    auto m = problem->getSolutionSize();
    auto n = problem->getProblemSize();
    double **matriz = realproblem->getMatriz();
    tSolution sol = solution;
    tFitness costeActual = fitness;
    int evaluaciones = 0;
    int intentos = 0;

    std::vector<tOption> seleccionados;
    for (int i = 0; i < n; ++i) {
        if (sol[i]) seleccionados.push_back(i);
    }

    bool mejora = true;

    while (mejora && evaluaciones < maxevals && intentos < 400) {
        mejora = false;

        std::vector<std::pair<tOption, tOption>> vecinos;
        for (size_t i = 0; i < seleccionados.size(); ++i) {
            for (tOption j = 0; j < n; ++j) {
                if (!sol[j]) {
                    vecinos.emplace_back(i, j);
                }
            }
        }

        std::shuffle(vecinos.begin(), vecinos.end(), Random::engine());

        for (auto [i, j] : vecinos) {
            if (evaluaciones >= maxevals) break;
            evaluaciones++;

            if (verifica(seleccionados, seleccionados[i], j, m)) {
                if (calcularCoste(seleccionados, i, j, realproblem, costeActual, sol)) {
                    seleccionados[i] = j;
                    costeActual = problem->fitness(sol);
                    mejora = true;
                    break; // Primera mejora
                }
            }
            intentos++;
        }
    }

    return ResultMH(sol, costeActual, evaluaciones);
}
