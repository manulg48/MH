#pragma once
#include "GeneticAlgorithm.h"
#include <vector>

/**
 * AGE (Stationary Genetic Algorithm)
 * Implementation specialized for solving the MDD problem.
 */
class AGE : public GeneticAlgorithm {
public:
    AGE(int populationSize = 50, double crossProb = 1.0, double mutationProb = 0.1);
    virtual ~AGE() {}

    // Override optimize from MHTrayectory
    ResultMH optimize(Problem *problem, const tSolution &initial,
                      tFitness fitness, int maxevals) override;

private:
    // Specific methods for stationary scheme
    void stationaryReplacement(const tSolution &offspring, Problem* problem);
    tSolution generateOffspring(const tSolution& parent1, const tSolution& parent2, Problem* problem);

    // Internal helper methods
    void runStationaryCycle(Problem *problem, int &evals, int maxevals);
};
