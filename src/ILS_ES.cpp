// ILS_ES.cpp
#include "ILS_ES.h"
#include <cassert>

ILS_ES::ILS_ES()
  : numIterations( /* TODO: e.g. 10 */ ),
    es()    // EnfriamientoSimulado interno
{}

ILS_ES::~ILS_ES() {}

ResultMH ILS_ES::optimize(Problem *problem, int maxevals) {
  // TODO: idéntico a ILS pero BL→es.optimize
  return ResultMH(  sol ,  fit ,  evals  );
}

ResultMH ILS_ES::optimize(Problem *problem,
                         const tSolution &current,
                         tFitness fitness,
                         int maxevals) {
  // TODO: versión “trayectoria” híbrida
  return ResultMH(  sol ,  fit ,  evals  );
}

void ILS_ES::mutar(const tSolution &orig, tSolution &mod) {
  // TODO: mutación “fuerte” sobre mod (20% de cambios)
}
