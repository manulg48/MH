// EnfriamientoSimulado.cpp
#include "EnfriamientoSimulado.h"
#include <cassert>

EnfriamientoSimulado::EnfriamientoSimulado()
  : T0( /* TODO: inicializar */ ),
    Tf( /* TODO: inicializar */ ),
    beta( /* TODO: inicializar */ ),
    maxVecinos( /* TODO: inicializar */ ),
    maxExitos( /* TODO: inicializar */ )
{}

EnfriamientoSimulado::~EnfriamientoSimulado() {}

ResultMH EnfriamientoSimulado::optimize(Problem *problem, int maxevals) {
  // TODO: implementar bucle principal de ES (inicializar solución,
  // iterar enfriamientos, generar vecinos, aceptar con probabilidad)
  return ResultMH(  sol ,  fit ,  evals );
}

ResultMH EnfriamientoSimulado::optimize(Problem *problem,
                                        const tSolution &current,
                                        tFitness fitness,
                                        int maxevals) {
  // TODO: versión “trayectoria” a partir de solución dada
  return ResultMH(  sol ,  fit ,  evals  );
}

double EnfriamientoSimulado::enfriar(double T) {
  // TODO: esquema de Cauchy modificado: Tk+1 = Tk / (1 + beta * Tk)
  return T;
}

bool EnfriamientoSimulado::aceptar(tFitness delta, double T) {
  // TODO: criterio de Metropolis
  return false;
}
