#ifndef MEMETICOS_H
#define MEMETICOS_H

#include "mhtrayectory.h"
#include "solution.h"
#include "problem.h"

class AM101 : public MHTrayectory {
public:
    ResultMH optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) override;
};

class AM1001 : public MHTrayectory {
public:
    ResultMH optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) override;
};

class AM1001Mej : public MHTrayectory {
public:
    ResultMH optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) override;
};

#endif // MEMETICOS_H
