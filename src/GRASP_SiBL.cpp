// GRASP_SiBL.cpp
#include "GRASP_SiBL.h"
#include <cassert>

GRASPSiBL::GRASPSiBL()
  : numSolutions( /* TODO: e.g. 10 */ ),
    bl()  // BúsquedaLocal para refinamiento
{}

GRASPSiBL::~GRASPSiBL() {}

ResultMH GRASPSiBL::optimize(Problem *problem, int maxevals) {
  // TODO:
  // 1. Para cada iteración:
  //    a) sol = construir(problem)
  //    b) res = bl.optimize(problem, sol, fitness(sol), ...)
  //    c) actualizar mejor
  return ResultMH(  sol ,  fit ,  evals  );
}

tSolution GRASPSiBL::construir(Problem *problem) {
  // TODO: misma construcción GRASP que en NoBL
  return tSolution();
}
