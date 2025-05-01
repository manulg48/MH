#pragma once
#include <mhtrayectory.h>
#include <vector>
#include <random>

/**
 * Genetic Algorithm implementation inheriting from MHTrayectory
 * for solving the MDD problem.
 */
class GeneticAlgorithm : public MHTrayectory {
public:
  GeneticAlgorithm(int populationSize = 50, double crossProb = 0.7, double mutationProb = 0.1);
  virtual ~GeneticAlgorithm() {}

  /**
   * Main entry point of the Genetic Algorithm from a given solution.
   */
  ResultMH optimize(Problem *problem, const tSolution &initial,
                    tFitness fitness, int maxevals) override;

private:
  // Core Genetic Algorithm methods
  void initializePopulation(Problem* problem);
  void evaluatePopulation(Problem* problem);
  void selectParents(std::vector<tSolution>& parents, Problem* problem);
  void crossover(const tSolution &parent1, const tSolution &parent2,
                 tSolution &child1, tSolution &child2, Problem* problem);
  void mutate(tSolution &solution, Problem* problem);
  void generationalReplacement(Problem* problem);
  void elitistReplacement(const tSolution &eliteSolution, tFitness eliteFitness);

  // Helper methods
  tSolution binaryToSolution(const std::vector<int>& binaryRepresentation, Problem* problem);
  std::vector<int> solutionToBinary(const tSolution& solution, Problem* problem);

  // GA Parameters
  int populationSize;
  double crossProb;
  double mutationProb;
  int chromosomeLength;
  int selectedNodes;

  // Population management
  std::vector<tSolution> population;
  std::vector<tFitness> fitnesses;

  // Tracking best solution
  tSolution bestSolution;
  tFitness bestFitness;

  // Random number generators
  std::mt19937 rng;
  std::uniform_real_distribution<double> realDist;
  std::uniform_int_distribution<int> intDist;

  // Initialization and selection methods
  tSolution generateRandomSolution(Problem* problem);
  int tournamentSelection(Problem* problem, int tournamentSize = 3);
};
