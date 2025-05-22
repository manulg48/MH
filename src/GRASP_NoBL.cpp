// GRASP_NoBL.cpp
#include "GRASP_NoBL.h"
#include <cassert>

GRASPNoBL::GRASPNoBL()
  : numSolutions( /* TODO: e.g. 10 */ )
{}

GRASPNoBL::~GRASPNoBL() {}

ResultMH GRASPNoBL::optimize(Problem *problem, int maxevals) {
  // TODO:
  // 1. Mejor global ← infinita
  // 2. Para i in [1..numSolutions]:
  //    a) sol_i = construir(problem)
  //    b) fit_i = problem->fitness(sol_i)
  //    c) actualizar mejor global
  return ResultMH(  sol ,  fit ,  evals  );
}

tSolution GRASPNoBL::construir(Problem *problem) {
  // TODO: procedimiento GRASP de construcción con LRC dinámica
  return tSolution();
}
