#pragma once
#include "mhtrayectory.h"
#include <mindiff.h>

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class BusquedaLocal : public MHTrayectory {

public:
  BusquedaLocal() : MHTrayectory() {}
  virtual ~BusquedaLocal() {}
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals) override;

  public:
  /**
   * Run the Trayectory-based metaheuristic algorithm to find the optimal
   * solution starting from a given solution.
   *
   * @param problem  The problem to solve.
   * @param current The initial solution.
   * @param fitness The fitness of the initial solution.
   * @param maxevals The maximum number of evaluations.
   */
  ResultMH optimize(Problem *problem, const tSolution &current,
                            tFitness fitness, int maxevals) override;
  bool verifica(const vector<tOption>& sel, tOption i, tOption j, int m);
  bool calcularCoste(vector<tOption> &sel, tOption i, tOption j, Mindiff *problema, tFitness costeActual, tSolution &sol);
  void actualizarSolucion(vector<tDomain> &sol,vector<tOption> & sel);

};