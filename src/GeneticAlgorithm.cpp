#include "GeneticAlgorithm.h"
#include <algorithm>
#include <numeric>
#include "random.hpp"

GeneticAlgorithm::GeneticAlgorithm(int _populationSize, double _crossProb, double _mutationProb){
    populationSize = _populationSize;
    crossProb = _crossProb;
    mutationProb = _mutationProb;
}
