#pragma once
#include "mhtrayectory.h"
#include "mindiff.h"

using namespace std;

/**
 * Búsqueda Local Reiterada (ILS)
 *  - Aplica BL a una solución inicial, muta la mejor solución
 *    y repite un nº fijo de iteraciones.
 *
 * @see MHTrayectory
 */
class ILS : public MHTrayectory {
public:
  ILS();
  virtual ~ILS();

  // Entrada estándar
  ResultMH optimize(Problem *problem, int maxevals) override;
  // Llamada interna desde otra trayectoria
  ResultMH optimize(Problem *problem,
                    const tSolution &current,
                    tFitness fitness,
                    int maxevals) override;

private:
  int numIterations;   // p.ej. 10 aplicaciones de BL :contentReference[oaicite:2]{index=2}
  int maxEvalsBL;      // evaluaciones límite para cada BL (p.ej. 10000)

  // Mutación “fuerte” (20% de elementos)
  void mutar(const tSolution &orig, tSolution &mod);
};
