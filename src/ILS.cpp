// ILS.cpp
#include "ILS.h"
#include "busquedalocal.h"
#include <limits>

ILS::ILS()
  : numIterations(10),    // 1 BL inicial + 9 mutaciones
    maxEvalsBL(10000)     // tope de 10000 evaluaciones por BL
{}

ILS::~ILS() {}


// 1) Sólo recibe (problem, maxevals): crea la inicial y delega
ResultMH ILS::optimize(Problem *problem, int /*maxevals*/) {
    // 1.1 Generar solución inicial aleatoria
    tSolution init = problem->createSolution();
    tFitness  fit0 = problem->fitness(init);

    // 1.2 Delegar en la otra sobrecarga
    return optimize(problem, init, fit0, maxEvalsBL);
}


// 2) Fase BL + mutaciones: usa numIterations y maxEvalsBL
ResultMH ILS::optimize(Problem *problem,
                       const tSolution &current,
                       tFitness fitness,
                       int /*ignored*/) {
    BusquedaLocal bl;

    // 2.1 BL inicial sobre 'current'
    ResultMH r0 = bl.optimize(problem, current, fitness, maxEvalsBL);
    tSolution bestSol  = r0.solution;
    tFitness  bestFit  = r0.fitness;
    int       totalEvals = r0.evaluations;

    // 2.2 Iteraciones ILS (9 mutaciones tras la inicial)
    for (int it = 1; it < numIterations; ++it) {
        // 2.2.1 Mutar la mejor hasta ahora
        tSolution cand;
        mutar(bestSol, cand);
        tFitness fitCand = problem->fitness(cand);

        // 2.2.2 Refine con BL, tope maxEvalsBL
        ResultMH r = bl.optimize(problem, cand, fitCand, maxEvalsBL);
        totalEvals += r.evaluations;

        // 2.2.3 Criterio “del mejor”
        if (r.fitness < bestFit) {
            bestFit = r.fitness;
            bestSol = r.solution;
        }
    }

    // 2.3 Devolver la mejor de todas
    return ResultMH(bestSol, bestFit, totalEvals);
}


// 3) Mutación: cambia el 20% de elementos por no seleccionados
void ILS::mutar(const tSolution &orig, tSolution &mod) {
    mod = orig;
    int n = orig.size();

    vector<int> sel, noSel;
    for (int i = 0; i < n; ++i)
        (orig[i] ? sel : noSel).push_back(i);

    int m = sel.size();
    int k = max(2, int(0.2 * m));              // mínimo 2
    k = min({k, m, int(noSel.size())});        // no pasarse

    shuffle(sel.begin(), sel.end(), Random::engine());
    shuffle(noSel.begin(), noSel.end(), Random::engine());

    for (int i = 0; i < k; ++i) {
        mod[sel[i]]    = false;
        mod[noSel[i]] = true;
    }
}
