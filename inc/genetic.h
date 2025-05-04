#pragma once

#include "mhtrayectory.h"
#include "solution.h"
#include "problem.h"
#include <vector>

enum class CrossoverType { UNIFORM, POSITION };
enum class EvolutionModel { GENERATIONAL, STATIONARY };

class GeneticAlgorithm : public MHTrayectory {
public:
    GeneticAlgorithm(size_t pop_size, double pc, double pm,
                     CrossoverType crossover_type, EvolutionModel evolution_model);

    ResultMH optimize(Problem* problem, const tSolution& current,
                      tFitness fitness, int maxevals) override;
    
    ResultMH optimize(Problem *problem, int maxevals) override;

private:
    size_t pop_size;
    double pc; // crossover probability
    double pm; // mutation probability
    CrossoverType crossover_type;
    EvolutionModel evolution_model;

    Problem* prob_ptr = nullptr;
    int evals = 0;
    int max_evals = 0;

    std::vector<tSolution> population;
    std::vector<tFitness> fitnesses;

    void initialize_population(const tSolution& initial, tFitness fitness);
    void evaluate_population();
    int tournament();
    std::pair<tSolution, tSolution> crossover(const tSolution& p1, const tSolution& p2, Problem* problem);
    void mutate(tSolution& s,Problem* problem);
    void replace_population(const std::vector<tSolution>& offspring, const std::vector<tFitness>& offspring_fit);
    int best_index();
};
