// GRASPSiBL.cpp

#include "GRASP_SiBL.h"
#include "busquedalocal.h"
#include "random.hpp"
#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>

GRASPSiBL::GRASPSiBL()
  : numSolutions(10),       // generamos 10 soluciones GRASP :contentReference[oaicite:0]{index=0}
    bl()                    // BL para refinamiento
{}

GRASPSiBL::~GRASPSiBL() {}

// ------------------------------------------------------------
// 1) Procedimiento GRASP: repetir construir+BL y quedarnos con la mejor :contentReference[oaicite:1]{index=1}
// ------------------------------------------------------------
ResultMH GRASPSiBL::optimize(Problem *problem, int maxevals) {
    tSolution bestSol;
    tFitness  bestFit = std::numeric_limits<tFitness>::infinity();
    int       totalEvals = 0;

    for (int it = 0; it < numSolutions; ++it) {
        // a) Construcción greedy aleatorizada
        tSolution sol = construir(problem);
        // b) Evaluamos su fitness
        tFitness fit0 = problem->fitness(sol);
        totalEvals++;  // una evaluación de fitness
        // c) Refinamos con BL (hasta maxevals por llamada)
        ResultMH r = bl.optimize(problem, sol, fit0, maxevals);
        totalEvals += r.evaluations;
        // d) Actualizamos la mejor solución
        if (r.fitness < bestFit) {
            bestFit = r.fitness;
            bestSol = r.solution;
        }
    }

    return ResultMH(bestSol, bestFit, totalEvals);
}


// ------------------------------------------------------------
// 2) Construcción GRASP para MDD: 
//    – Dos nodos iniciales aleatorios :contentReference[oaicite:2]{index=2}  
//    – LRC variable con umbral μ = dmin + α·(dmax−dmin), α=0.2 :contentReference[oaicite:3]{index=3}  
//    – Repetir m-2 pasos más para completar la solución
// ------------------------------------------------------------
tSolution GRASPSiBL::construir(Problem *problem) {
    int n = problem->getProblemSize();    // total de nodos
    int m = problem->getSolutionSize();   // nodos a escoger
    const double alpha = 0.2;             // umbral :contentReference[oaicite:4]{index=4}

    // 1) Inicialización
    tSolution sol(n,false);
    std::vector<int> CL(n);
    std::iota(CL.begin(), CL.end(), 0);

    // 2) Elegimos 2 nodos aleatorios y los eliminamos de CL
    auto pick_and_remove = [&](int &out) {
        int idx = Random::get<int>(0, CL.size() - 1);
        out = CL[idx];
        sol[out] = true;
        CL.erase(CL.begin() + idx);
    };
    int v0, v1;
    pick_and_remove(v0);
    pick_and_remove(v1);

    // 3) Dispersion acumulada de cada candidato a Sel={v0,v1}
    std::vector<double> disp(n, 0.0);
    for (int i = 0; i < n; ++i) {
        if (!sol[i]) {
            disp[i] = problem->getDistancia(i, v0)
                    + problem->getDistancia(i, v1);
        }
    }

    // 4) Construir el resto de la solución hasta |Sel|=m
    std::vector<int> S = {v0, v1};
    while ((int)S.size() < m) {
        // 4.1) Calcular dmin/dmax sobre los candidatos CL
        double dmin = std::numeric_limits<double>::infinity();
        double dmax = std::numeric_limits<double>::lowest();
        for (int c : CL) {
            dmin = std::min(dmin, disp[c]);
            dmax = std::max(dmax, disp[c]);
        }
        double mu = dmin + alpha * (dmax - dmin);

        // 4.2) Formar LRC = { pos ∈ [0..|CL|-1] | disp[ CL[pos] ] ≤ μ }
        std::vector<int> LRC;
        for (int pos = 0; pos < (int)CL.size(); ++pos) {
            if (disp[CL[pos]] <= mu)
                LRC.push_back(pos);
        }

        // 4.3) Escoger aleatoriamente uno de la LRC
        int selPos = LRC[ Random::get<int>(0, LRC.size() - 1) ];
        int sel    = CL[selPos];
        sol[sel]   = true;
        S.push_back(sel);
        CL.erase(CL.begin() + selPos);

        // 4.4) Actualizar dispersiones de los restantes
        for (int c : CL) {
            disp[c] += problem->getDistancia(c, sel);
        }
    }

    return sol;
}

ResultMH optimize(Problem *problem,
                       const tSolution &current,
                       tFitness fitness,
                       int /*ignored*/){
                        tSolution bestSol;
    tFitness  bestFit = std::numeric_limits<tFitness>::infinity();
    int       totalEvals = 0;
                        return ResultMH(bestSol, bestFit, totalEvals);
                       } 
