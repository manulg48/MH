// BMB.cpp
#include "BMB.h"

BMB::BMB()
  : numRestarts(10000),
    bl() 
{}

BMB::~BMB() {}

ResultMH BMB::optimize(Problem *problem, int maxevals) {
  int evalPerRestart = maxevals / numRestarts;
  double bestFitness = numeric_limits<double>::infinity();
  tSolution bestSol;
  unsigned totalEvals = 0;

  for (int i = 0;i < numRestarts;i++){
    tSolution sol = problem->createSolution();
    tFitness fit = problem->fitness(sol);

    ResultMH res = bl.optimize(problem,sol,fit,evalPerRestart);

    totalEvals += res.evaluations;

    if (res.fitness < bestFitness){
      bestFitness = res.fitness;
      bestSol = res.solution;
    }
  }

  return ResultMH( sol ,  fit ,  evals );
}
