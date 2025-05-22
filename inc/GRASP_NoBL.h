#pragma once
#include "mh.h"
#include "mindiff.h"

using namespace std;

/**
 * GRASP-NOBL
 *  - Construye varias soluciones greedy aleatorizadas y devuelve la mejor,
 *    sin aplicar búsqueda local.
 *
 * @see MH
 */
class GRASPNoBL : public MH {
public:
  GRASPNoBL();
  virtual ~GRASPNoBL();

  ResultMH optimize(Problem *problem, int maxevals) override;

private:
  int numSolutions;  // p.ej. 10 construcciones :contentReference[oaicite:5]{index=5}

  // Construye una solución parcial con LRC dinámico
  tSolution construir(Problem *problem);
};
