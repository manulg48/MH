// EnfriamientoSimulado.h
#pragma once
#include "mhtrayectory.h"
#include <vector>

class EnfriamientoSimulado : public MHTrayectory {
public:
    EnfriamientoSimulado();
    ~EnfriamientoSimulado() override;

    // maxevals = 100000 en nuestras pruebas
    ResultMH optimize(Problem *problem, int maxevals) override;
ResultMH optimize(Problem *problem,
                       const tSolution &current,
                       tFitness fitness,
                       int /*ignored*/) override;
private:
    // Genera un vecino por intercambio brusco (mismas reglas que BL)
    void vecino(const tSolution &orig, tSolution &mod);
};
