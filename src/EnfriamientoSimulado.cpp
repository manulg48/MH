// EnfriamientoSimulado.cpp
#include "EnfriamientoSimulado.h"
#include "random.hpp"
#include <cmath>
#include <limits>

EnfriamientoSimulado::EnfriamientoSimulado() {}
EnfriamientoSimulado::~EnfriamientoSimulado() {}

ResultMH EnfriamientoSimulado::optimize(Problem *problem, int maxevals) {
    // 1) Solución inicial y su fitness
    tSolution sol     = problem->createSolution();
    tFitness  fit     = problem->fitness(sol);
    int        evals  = 1;

    // 2) Guardamos la mejor global
    tSolution bestSol = sol;
    tFitness  bestFit = fit;

    // 3) Parámetros de enfriamiento (Seminario 4, diapositiva 18)
    const double phi   = 0.3;
    const double mu    = 0.2;
    const double Tf    = 1e-3;                      // temperatura final :contentReference[oaicite:0]{index=0}
    int    m           = problem->getSolutionSize();
    int    max_vecinos = 100 * m;                   // tamaño L(T) :contentReference[oaicite:1]{index=1}
    int    max_exitos  = int(0.1 * max_vecinos);

    // 3.1) Número de enfriamientos M = maxevals / max_vecinos :contentReference[oaicite:2]{index=2}
    int M = std::ceil(double(maxevals) / max_vecinos);

    // 3.2) Temperatura inicial T0 = μ·Coste(S0)/(-ln φ) :contentReference[oaicite:3]{index=3}
    double T0 = mu * fit / (-std::log(phi));
    if (T0 <= Tf) T0 = Tf * 10;
    // 3.3) Beta para esquema de Cauchy modificado: T_{k+1} = T_k / (1 + β·T_k)
    double beta = (T0 - Tf) / (M * T0 * Tf);

    double T = T0;

    // 4) Bucle externo de enfriamiento
    for (int k = 0; k < M && evals < maxevals; ++k) {
        int n_vec = 0, n_ex = 0;

        // 4.1) Bucle interno L(T): hasta max_vecinos o max_exitos o maxevals
        while (n_vec < max_vecinos && n_ex < max_exitos && evals < maxevals) {
            tSolution cand;
            vecino(sol, cand);

            tFitness fit2 = problem->fitness(cand);
            ++evals; ++n_vec;

            double delta = fit2 - fit; // queremos minimizar
            if (delta <= 0.0
             || std::uniform_real_distribution<>(0.0,1.0)(Random::engine())
                < std::exp(-delta / T)) {
                // 4.2) Aceptamos el vecino
                sol = cand;
                fit = fit2;
                ++n_ex;

                // 4.3) Actualizamos mejor global si procede
                if (fit2 < bestFit) {
                    bestFit = fit2;
                    bestSol = cand;
                }
            }
        }

        // 4.4) Si en L(T) no hubo éxitos, detenemos
        if (n_ex == 0) break;

        // 4.5) Enfriamiento por Cauchy modificado
        T = T / (1.0 + beta * T);
        if (T < Tf) break;
    }

    return ResultMH(bestSol, bestFit, evals);
}

void EnfriamientoSimulado::vecino(const tSolution &orig, tSolution &mod) {
    // Igual que en BL: intercambiamos un 1 por un 0 al azar
    mod = orig;
    int n = orig.size();
    std::vector<int> sel, noSel;
    for (int i = 0; i < n; ++i)
        (orig[i] ? sel : noSel).push_back(i);

    if (sel.empty() || noSel.empty()) return;

    int i = sel[ Random::get<int>(0, sel.size()-1) ];
    int j = noSel[ Random::get<int>(0, noSel.size()-1) ];
    mod[i] = false;
    mod[j] = true;
}
