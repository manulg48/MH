// ILS_ES.h
#pragma once
#include "mhtrayectory.h"
#include "EnfriamientoSimulado.h"

class ILS_ES : public MHTrayectory {
public:
    ILS_ES(int numIters = 10, int saEvals = 10000);
    ~ILS_ES() override;

    ResultMH optimize(Problem *problem, int /*maxevals*/) override;
ResultMH optimize(Problem *problem,
                       const tSolution &current,
                       tFitness fitness,
                       int /*ignored*/) override {};
private:
    int numIterations;      // número total de llamadas a SA (1 inicial + numIters-1 mutadas)
    int maxEvalsSA;         // máximo de evaluaciones por llamada a SA
    EnfriamientoSimulado sa; 
    void mutar(const tSolution &orig, tSolution &mod);
};
