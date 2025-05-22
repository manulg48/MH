// BMB.cpp
#include "BMB.h"

BMB::BMB()
  : numRestarts( /* TODO: e.g. 10 */ ),
    bl() 
{}

BMB::~BMB() {}

ResultMH BMB::optimize(Problem *problem, int maxevals) {
  // TODO: 
  // 1. Mejor global ← infinita
  // 2. Para i in [1..numRestarts]:
  //    a) crear solución aleatoria
  //    b) aplicar bl.optimize(sobre maxevals/numRestarts)
  //    c) comparar y actualizar mejor global
  // 3. Devolver mejor global
  return ResultMH( sol ,  fit ,  evals );
}
