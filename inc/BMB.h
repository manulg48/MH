#pragma once
#include "mh.h"
#include "mindiff.h"
#include "busquedalocal.h"
#include "solution.h"

using namespace std;

/**
 * Búsqueda Multiarranque Básica (BMB)
 *  - Genera varias soluciones aleatorias y sobre cada una aplica BL,
 *    devolviendo la mejor.
 *
 * @see MH
 * @see BusquedaLocal
 */
class BMB : public MHTrayectory {
public:
  BMB();
  virtual ~BMB();

  // Sólo necesita la interfaz global
  ResultMH optimize(Problem *problem, int maxevals) override;

private:
  int numRestarts;    // nº de arranques (p.ej. 10)
  BusquedaLocal bl;   // algoritmo de BL para refinar soluciones
  tSolution sol;
  tFitness fit;
  int evals;
};
