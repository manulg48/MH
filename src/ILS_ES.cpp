// ILS_ES.cpp
#include "ILS_ES.h"
#include <limits>
#include <algorithm>
#include <random.hpp>

ILS_ES::ILS_ES(int numIters, int saEvals)
  : numIterations(numIters),
    maxEvalsSA(saEvals),
    sa()
{}

ILS_ES::~ILS_ES() {}

ResultMH ILS_ES::optimize(Problem *problem, int /*ignored*/) {
    // 1) Generamos solución inicial aleatoria
    tSolution init = problem->createSolution();
    tFitness  fit0 = problem->fitness(init);

    // 2) Refinamos con SA la inicial
    ResultMH r0 = sa.optimize(problem, init, fit0, maxEvalsSA);
    tSolution bestSol = r0.solution;
    tFitness  bestFit = r0.fitness;
    int       totalEvals = r0.evaluations;

    // 3) Bucle ILS: mutar + SA
    for (int it = 1; it < numIterations; ++it) {
        // 3.1) mutar la mejor hasta ahora
        tSolution cand;
        mutar(bestSol, cand);

        // 3.2) evaluar el candidato
        tFitness fitCand = problem->fitness(cand);
        totalEvals++;

        // 3.3) refinar con SA
        ResultMH r = sa.optimize(problem, cand, fitCand, maxEvalsSA);
        totalEvals += r.evaluations;

        // 3.4) aceptar si mejora
        if (r.fitness < bestFit) {
            bestFit = r.fitness;
            bestSol = r.solution;
        }
    }

    return ResultMH(bestSol, bestFit, totalEvals);
}

void ILS_ES::mutar(const tSolution &orig, tSolution &mod) {
    mod = orig;
    int n = orig.size();
    std::vector<int> sel, noSel;
    for (int i = 0; i < n; ++i)
        (orig[i] ? sel : noSel).push_back(i);

    int m = sel.size();
    int k = std::max(2, int(0.2 * m));
    k = std::min({k, m, int(noSel.size())});

    std::shuffle(sel.begin(), sel.end(), Random::engine());
    std::shuffle(noSel.begin(), noSel.end(), Random::engine());

    for (int i = 0; i < k; ++i) {
        mod[sel[i]]    = false;
        mod[noSel[i]] = true;
    }
}
