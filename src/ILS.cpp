// ILS.cpp
#include "ILS.h"
#include <cassert>

ILS::ILS()
  : numIterations( /* TODO: e.g. 10 */ ),
    maxEvalsBL( /* TODO: e.g. 10000 */ )
{}

ILS::~ILS() {}

ResultMH ILS::optimize(Problem *problem, int maxevals) {
  // TODO: 
  // 1. generar solución inicial aleatoria
  // 2. aplicar BL (optimize(problem, sol, fit, …))
  // 3. repetir numIterations veces: mutar, BL, comparar mejor
  return ResultMH(  sol ,  fit ,  evals  );
}

ResultMH ILS::optimize(Problem *problem,
                      const tSolution &current,
                      tFitness fitness,
                      int maxevals) {
  // TODO: misma lógica pero partiendo de (current, fitness)
  return ResultMH(  sol ,  fit ,  evals  );
}

void ILS::mutar(const tSolution &orig, tSolution &mod) {
  // TODO: cambiar el 20% de los elementos por otros no seleccionados
}
