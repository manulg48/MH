// GRASP_NoBL.cpp
#include "GRASP_NoBL.h"
#include "GRASP_SiBL.h"
#include <cassert>

GRASPNoBL::GRASPNoBL()
  : numSolutions( 10)
{}

GRASPNoBL::~GRASPNoBL() {}

ResultMH GRASPNoBL::optimize(Problem *problem, int /*maxevals*/) {
    tSolution bestSol;
    tFitness  bestFit = std::numeric_limits<tFitness>::infinity();
    int       totalEvals = 0;

    for (int it = 0; it < numSolutions; ++it) {
        // a) Construcción greedy aleatorizada
        tSolution sol = construir(problem);

        // b) Evaluamos su fitness UNA vez
        tFitness fit0 = problem->fitness(sol);
        totalEvals++;

        // c) Actualizamos la mejor solución con fit0
        if (fit0 < bestFit) {
            bestFit = fit0;
            bestSol = sol;
        }
    }

    return ResultMH(bestSol, bestFit, totalEvals);
}
