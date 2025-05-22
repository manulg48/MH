#pragma once
#include "mh.h"
#include "mindiff.h"
#include "busquedalocal.h"

using namespace std;

/**
 * GRASP-SIBL
 *  - Igual que GRASP, pero tras cada construcción aplica BL
 *    para refinar la solución antes de comparar.
 *
 * @see MH
 * @see BusquedaLocal
 */
class GRASPSiBL : public MH {
public:
  GRASPSiBL();
  virtual ~GRASPSiBL();

  ResultMH optimize(Problem *problem, int maxevals) override;

private:
  int numSolutions;    // p.ej. 10 iteraciones
  BusquedaLocal bl;    // BL a aplicar tras cada construcción

  tSolution sol;
  tFitness fit;
  int evals;
  tSolution construir(Problem *problem);
};
