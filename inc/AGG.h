#pragma once
#include "GeneticAlgorithm.h"
#include <vector>

/**
 * AGG (Generational Genetic Algorithm with Elitism) 
 * Implementation specialized for solving the MDD problem.
 */
class AGG : public GeneticAlgorithm {
public:
    AGG(int populationSize = 50, double crossProb = 0.7, double mutationProb = 0.1);
    virtual ~AGG() {}

    // Override optimize from MHTrayectory
    ResultMH optimize(Problem *problem, const tSolution &initial,
                      tFitness fitness, int maxevals) override;

private:
    // Specific methods for generational scheme
    void generationalReplacement(const std::vector<tSolution>& offspring, Problem* problem);
    void applyElitism(const tSolution& eliteSolution, tFitness eliteFitness);

    // Internal helper methods
    void runGenerationalCycle(Problem *problem, int &evals, int maxevals);
};
