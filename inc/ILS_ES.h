#pragma once
#include "mhtrayectory.h"
#include "EnfriamientoSimulado.h"
#include "mindiff.h"

using namespace std;

/**
 * Hibridación ILS-ES
 *  - Igual que ILS, pero en lugar de BL usa ES como refinamiento interno.
 *
 * @see MHTrayectory
 * @see EnfriamientoSimulado
 */
class ILS_ES : public MHTrayectory {
public:
  ILS_ES();
  virtual ~ILS_ES();

  ResultMH optimize(Problem *problem, int maxevals) override;
  ResultMH optimize(Problem *problem,
                    const tSolution &current,
                    tFitness fitness,
                    int maxevals) override;

private:
  int numIterations;         // nº de iteraciones totales
  EnfriamientoSimulado es;   // refinamiento interno con ES

  tSolution sol;
  tFitness fit;
  int evals;
  
  void mutar(const tSolution &orig, tSolution &mod);
};
