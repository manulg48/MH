#pragma once
#include "mhtrayectory.h"
#include "mindiff.h"

using namespace std;

/**
 * GRASP-NOBL
 *  - Construye varias soluciones greedy aleatorizadas y devuelve la mejor,
 *    sin aplicar búsqueda local.
 *
 * @see MH
 */
class GRASPNoBL : public MHTrayectory {
public:
  GRASPNoBL();
  virtual ~GRASPNoBL();

  ResultMH optimize(Problem *problem, int maxevals) override;
ResultMH optimize(Problem *problem,
                       const tSolution &current,
                       tFitness fitness,
                       int /*ignored*/) override {};
private:
  int numSolutions;  // p.ej. 10 construcciones :contentReference[oaicite:5]{index=5}

  tSolution sol;
  tFitness fit;
  int evals;

};
