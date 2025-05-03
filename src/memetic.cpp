#include "memetic.h"
#include "busquedalocal.h"
#include "random.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

struct Individuo {
    tSolution sol;
    tFitness fit;

    bool operator<(const Individuo &other) const {
        return fit < other.fit;
    }
};

static Individuo torneo3(const std::vector<Individuo> &pobla) {
    auto a = pobla[Random::get<int>(0, pobla.size() - 1)];
    auto b = pobla[Random::get<int>(0, pobla.size() - 1)];
    auto c = pobla[Random::get<int>(0, pobla.size() - 1)];
    return std::min({a, b, c});
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

static void mutarIndividuos(std::vector<Individuo> &hijos, Problem *problem, int cantidad) {
    int n,m;
    n = problem->getProblemSize();
    m = problem->getSolutionSize();
    for (int i = 0; i < cantidad; ++i) {
        int idx = Random::get<int>(0, hijos.size() - 1);
        mutate(hijos[idx].sol,m,n);
    }
}

static void aplicarBLCompleta(std::vector<Individuo> &pobla, Problem *problem, int &evals, int maxevals) {
    for (auto &ind : pobla) {
        BusquedaLocal bl;
        ResultMH res = bl.optimize(problem,ind.sol,ind.fit,maxevals - evals);
        ind.sol = res.solution;
        ind.fit = res.fitness;
        evals += res.evaluations;
    }
}

static void aplicarBLProbabilistica(std::vector<Individuo> &pobla, Problem *problem, int &evals, int maxevals, double prob) {
    for (auto &ind : pobla) {
        if (Random::get<bool>(prob)) {
            BusquedaLocal bl;
            ResultMH res = bl.optimize(problem,ind.sol,ind.fit,maxevals - evals);
            ind.sol = res.solution;
            ind.fit = res.fitness;
            evals += res.evaluations;
        }
    }
}

static void aplicarBLMejores(std::vector<Individuo> &pobla, Problem *problem, int &evals, int maxevals, double porcentaje) {
    int cuantos = std::ceil(porcentaje * pobla.size());
    std::partial_sort(pobla.begin(), pobla.begin() + cuantos, pobla.end());
    for (int i = 0; i < cuantos; ++i) {
        BusquedaLocal bl;
        ResultMH res = bl.optimize(problem, pobla[i].sol, pobla[i].fit, maxevals - evals);
        pobla[i].sol = res.solution;
        pobla[i].fit = res.fitness;
        evals += res.evaluations;
    }
}

static ResultMH ejecutarAM(Problem *problem, const tSolution &current, tFitness fitness, int maxevals,
                           void (*aplicarBL)(std::vector<Individuo>&, Problem*, int&, int),
                           double prob = 0.0) {
    const int POP_SIZE = 50;
    const int GENERACIONES_BL = 10;
    const double PC = 0.7;
    const double PM = 0.1;

    std::vector<Individuo> poblacion(POP_SIZE);
    int evaluaciones = 0;

    poblacion[0] = {current, fitness};
    evaluaciones++;
    for (int i = 1; i < POP_SIZE; ++i) {
        problem->initialize(poblacion[i].sol);
        poblacion[i].fit = problem->evaluate(poblacion[i].sol);
        evaluaciones++;
    }

    int generacion = 0;
    Individuo mejor = *std::min_element(poblacion.begin(), poblacion.end());

    while (evaluaciones < maxevals) {
        generacion++;
        std::vector<Individuo> padres;
        for (int i = 0; i < POP_SIZE; ++i)
            padres.push_back(torneo3(poblacion));

        std::vector<Individuo> hijos(POP_SIZE);
        int cruces = std::ceil(PC * POP_SIZE / 2);
        for (int i = 0; i < cruces * 2; i += 2)
            problem->crossover(padres[i].sol, padres[i + 1].sol, hijos[i].sol, hijos[i + 1].sol);

        for (int i = cruces * 2; i < POP_SIZE; ++i)
            hijos[i] = padres[i];

        mutarIndividuos(hijos, problem, std::ceil(PM * POP_SIZE));

        for (auto &h : hijos) {
            h.fit = problem->evaluate(h.sol);
            evaluaciones++;
        }

        auto peorIt = std::max_element(hijos.begin(), hijos.end());
        if (mejor.fit < peorIt->fit) *peorIt = mejor;
        poblacion = hijos;
        mejor = *std::min_element(poblacion.begin(), poblacion.end());

        if (generacion % GENERACIONES_BL == 0)
            aplicarBL(poblacion, problem, evaluaciones, maxevals);
    }

    return {mejor.sol, mejor.fit, evaluaciones};
}

ResultMH AM101::optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) {
    return ejecutarAM(problem, current, fitness, maxevals, aplicarBLCompleta);
}

ResultMH AM1001::optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) {
    return ejecutarAM(problem, current, fitness, maxevals,
                      [](std::vector<Individuo> &p, Problem *pr, int &e, int m) {
                          aplicarBLProbabilistica(p, pr, e, m, 0.1);
                      });
}

ResultMH AM1001Mej::optimize(Problem *problem, const tSolution &current, tFitness fitness, int maxevals) {
    return ejecutarAM(problem, current, fitness, maxevals,
                      [](std::vector<Individuo> &p, Problem *pr, int &e, int m) {
                          aplicarBLMejores(p, pr, e, m, 0.1);
                      });
}
