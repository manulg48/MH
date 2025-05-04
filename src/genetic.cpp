#include "genetic.h"
#include "random.hpp"
#include <algorithm>

void mutatePopulation(std::vector<tSolution>& population, double pm) {
    int n = population[0].size();

    for (auto& ind : population) {
        if (Random::get<double>() < pm) {
            std::vector<int> ones, zeros;
            for (int i = 0; i < n; ++i) {
                if (ind[i] == 1) ones.push_back(i);
                else zeros.push_back(i);
            }

            if (!ones.empty() && !zeros.empty()) {
                int quitar = Random::get<int>(0, ones.size() - 1);
                int poner = Random::get<int>(0, zeros.size() - 1);
                ind[ones[quitar]] = 0;
                ind[zeros[poner]] = 1;
            }
        }
    }
}


std::pair<tSolution, tSolution> crossoverUniform(const tSolution& p1, const tSolution& p2, int m, int n) {
    tSolution h1(n, 0), h2(n, 0);

    for (int i = 0; i < n; ++i) {
        if (p1[i] == 1 && p2[i] == 1) {
            h1[i] = h2[i] = 1;
        } else if (p1[i] != p2[i]) {
            if (Random::get<bool>()) h1[i] = 1;
            else h2[i] = 1;
        }
    }

    auto repair = [&](tSolution& s) {
        int count = std::count(s.begin(), s.end(), 1);
        if (count > m) {
            std::vector<int> ones;
            for (int i = 0; i < n; ++i)
                if (s[i] == 1) ones.push_back(i);
            Random::shuffle(ones);
            for (int i = 0; i < count - m; ++i)
                s[ones[i]] = 0;
        } else if (count < m) {
            std::vector<int> zeros;
            for (int i = 0; i < n; ++i)
                if (s[i] == 0) zeros.push_back(i);
            Random::shuffle(zeros);
            for (int i = 0; i < m - count; ++i)
                s[zeros[i]] = 1;
        }
    };

    repair(h1);
    repair(h2);

    return {h1, h2};
}

std::pair<tSolution, tSolution> crossoverPosition(const tSolution& p1, const tSolution& p2, int m, int n) {
    tSolution h1(n, 0), h2(n, 0);

    for (int i = 0; i < n; ++i) {
        if (p1[i] == 1 && p2[i] == 1)
            h1[i] = h2[i] = 1;
    }

    auto assign_remaining = [&](tSolution& h) {
        std::vector<int> available;
        for (int i = 0; i < n; ++i) {
            if ((p1[i] == 1 || p2[i] == 1) && h[i] == 0)
                available.push_back(i);
        }
        Random::shuffle(available);
        int needed = m - std::count(h.begin(), h.end(), 1);
        for (int i = 0; i < needed && i < (int)available.size(); ++i)
            h[available[i]] = 1;
    };

    assign_remaining(h1);
    assign_remaining(h2);

    return {h1, h2};
}

void mutate(tSolution& s, int m, int n) {
    std::vector<int> ones, zeros;
    for (int i = 0; i < n; ++i) {
        if (s[i] == 1) ones.push_back(i);
        else zeros.push_back(i);
    }

    if (ones.empty() || zeros.empty()) return;

    int i = ones[Random::get(0, (int)ones.size() - 1)];
    int j = zeros[Random::get(0, (int)zeros.size() - 1)];
    s[i] = 0;
    s[j] = 1;
}


GeneticAlgorithm::GeneticAlgorithm(size_t pop_size, double pc, double pm,
                                   CrossoverType crossover_type, EvolutionModel evolution_model)
    : pop_size(pop_size), pc(pc), pm(pm),
      crossover_type(crossover_type), evolution_model(evolution_model) {}

ResultMH GeneticAlgorithm::optimize(Problem* problem, const tSolution& current,
                                    tFitness fitness, int maxevals) {
    prob_ptr = problem;
    max_evals = maxevals;
    evals = 0;

    initialize_population(current, fitness);

    while (evals < max_evals) {
        std::vector<tSolution> offspring;
        std::vector<tFitness> offspring_fit;

        
        if (evolution_model == EvolutionModel::GENERATIONAL) {
            for (size_t i = 0; i < pop_size / 2; ++i) {
                tSolution p1 = population[tournament()];
                tSolution p2 = population[tournament()];
                auto [h1, h2] = crossover(p1, p2, problem);
                offspring.push_back(h1);
                offspring.push_back(h2);
            }

            // Aplicar mutación por generación completa
            mutatePopulation(offspring, pm);

            // Evaluar descendencia
            offspring_fit.clear();
            for (auto& ind : offspring) {
                offspring_fit.push_back(prob_ptr->fitness(ind));
                ++evals;
            }

            // Elitismo: preservar el mejor de la generación anterior
            int best_idx = best_index();
            tSolution best_sol = population[best_idx];
            tFitness best_fit = fitnesses[best_idx];

            int worst_idx = std::distance(offspring_fit.begin(),
                                        std::max_element(offspring_fit.begin(), offspring_fit.end()));
            if (offspring_fit[worst_idx] > best_fit) {
                offspring[worst_idx] = best_sol;
                offspring_fit[worst_idx] = best_fit;
            }

            replace_population(offspring, offspring_fit);

                } 
        else { // STATIONARY
            tSolution p1 = population[tournament()];
            tSolution p2 = population[tournament()];
            auto [h1, h2] = crossover(p1, p2, problem);

            if (Random::get(0.0, 1.0) < pm) mutate(h1, problem);
            if (Random::get(0.0, 1.0) < pm) mutate(h2, problem);

            tFitness f1 = prob_ptr->fitness(h1); ++evals;
            tFitness f2 = prob_ptr->fitness(h2); ++evals;


                    int worst_idx = std::distance(fitnesses.begin(),
                        std::max_element(fitnesses.begin(), fitnesses.end()));
                    if (f1 < fitnesses[worst_idx]) {
                        population[worst_idx] = h1;
                        fitnesses[worst_idx] = f1;
                    }
                    if (f2 < fitnesses[worst_idx]) {
                        population[worst_idx] = h2;
                        fitnesses[worst_idx] = f2;
                    }
                }
    }

    int best = best_index();
    return ResultMH{population[best], fitnesses[best], evals};
}

ResultMH GeneticAlgorithm::optimize(Problem *problem, int maxevals) {
    tSolution initial = problem->createSolution();
    tFitness fit = problem->fitness(initial);
    int evals = 1;

    return optimize(problem, initial, fit, maxevals - evals);
}

void GeneticAlgorithm::initialize_population(const tSolution& initial, tFitness fitness) {
    population.clear();
    fitnesses.clear();
    population.push_back(initial);
    fitnesses.push_back(fitness);
    evals = 1;

    while (population.size() < pop_size) {
        tSolution s = prob_ptr->createSolution();
        tFitness f = prob_ptr->fitness(s);
        population.push_back(s);
        fitnesses.push_back(f);
        ++evals;
    }
}

int GeneticAlgorithm::tournament() {
    int best = -1;
    for (int i = 0; i < 3; ++i) {
        int idx = Random::get(0, (int)population.size() - 1);
        if (best == -1 || fitnesses[idx] < fitnesses[best])
            best = idx;
    }
    return best;
}

std::pair<tSolution, tSolution> GeneticAlgorithm::crossover(const tSolution& p1, const tSolution& p2, Problem* problem) {
    int n,m;
    n = problem->getProblemSize();
    m = problem->getSolutionSize();
    if (crossover_type == CrossoverType::UNIFORM) {
        return crossoverUniform(p1, p2,m,n); // define esto en problem.h
    } else {
        return crossoverPosition(p1, p2,m,n); // define esto también
    }
}

void GeneticAlgorithm::mutate(tSolution& s,Problem* problem) {
    if (Random::get(0.0,1.0) < pm) {
        std::vector<int> activos,inactivos;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i])
                activos.push_back(i);
            else
                inactivos.push_back(i);
        }

        if (!activos.empty() && !inactivos.empty()) {
            int idx_act = activos[Random::get(0, (int)activos.size() - 1)];
            int idx_inact = inactivos[Random::get(0, (int)inactivos.size() - 1)];

            // Intercambio
            s[idx_act] = false;
            s[idx_inact] = true;
        }
    }
}

void GeneticAlgorithm::replace_population(const std::vector<tSolution>& offspring, const std::vector<tFitness>& offspring_fit) {
    int best_old = best_index();
    tSolution elite = population[best_old];
    tFitness elite_fit = fitnesses[best_old];

    population = offspring;
    fitnesses = offspring_fit;

    // Elitismo
    int worst = std::distance(fitnesses.begin(),
        std::max_element(fitnesses.begin(), fitnesses.end()));
    if (elite_fit < fitnesses[worst]) {
        population[worst] = elite;
        fitnesses[worst] = elite_fit;
    }
}

int GeneticAlgorithm::best_index() {
    return std::distance(fitnesses.begin(),
        std::min_element(fitnesses.begin(), fitnesses.end()));
}


