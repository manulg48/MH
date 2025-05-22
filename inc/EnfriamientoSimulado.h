#pragma once
#include "mhtrayectory.h"
#include <mindiff.h>

using namespace std;

/**
 * Enfriamiento Simulado (ES)
 *  - Permite aceptar peores soluciones según una temperatura que disminuye
 *    siguiendo un esquema de Cauchy modificado.
 *
 * @see MHTrayectory
 * @see Problem
 */
class EnfriamientoSimulado : public MHTrayectory {
public:
  EnfriamientoSimulado();
  virtual ~EnfriamientoSimulado();

  // Optimización global
  ResultMH optimize(Problem *problem, int maxevals) override;
  // Punto de entrada para llamada desde otra trayectoria o ILS
  ResultMH optimize(Problem *problem,
                    const tSolution &current,
                    tFitness fitness,
                    int maxevals) override;

private:
  double T0;           // temperatura inicial
  double Tf;           // temperatura final
  double beta;         // parámetro de enfriamiento
  int maxVecinos;      // número máximo de vecinos por temperatura
  int maxExitos;       // número máximo de éxitos por temperatura

  // Esquema de enfriamiento: Tk+1 = Tk / (1 + beta * Tk)
  double enfriar(double T);
  // Criterio de aceptación (Δf, T) → bool
  bool aceptar(tFitness delta, double T);
};
