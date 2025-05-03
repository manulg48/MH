#pragma once
#include "problem.h"

using namespace std;

class Mindiff : public Problem{
    private:
        int n;
        int m;
        double dis_max;
        double dis_min;
        double *distancias;
        double **matriz;
        tFitness costeActual;
    public:
        Mindiff(const string& path);
        ~Mindiff(){};
        /***
        * Evaluate the solution from scratch.
        * @param solution to evaluate.
        */
       tFitness fitness(const tSolution &solution) override;
       /**
        * Evaluate the solution indicating the current position to change and the new
        * value.
        *
        * By default it run the previous one. However, it can be override to
        * factorize the fitness.
        *
        * @param solution to evaluate.
        * @param solution_info information of solution to calculate the factored
        * fitness.
        * @param pos_change position of the solution to change.
        * @param new_value to store in pos_previous.
        */
       virtual tFitness fitness(const tSolution &solution,SolutionFactoringInfo *solution_info,unsigned pos_change, tDomain new_value) override;
        /**
        * Create a new solution.
        */
        tSolution createSolution() override;
        /**
        * Return the current size of the solution.
        */
        size_t getSolutionSize();
        /** Return the range of domain of each element of the solution */
        pair<tDomain, tDomain> getSolutionDomainRange() override;
    
        //Imprime la matriz
        void imprimirMatriz();

        double** getMatriz();

        double * getDistancias();
        /**
        *  Devuelve el tamañano del problema
        * @return n Tamaño del problema 
        */
        size_t getProblemSize() override;

        /**
         * Calcula la suma de las distancioas de un punto elegido al resto
         * @param p punto elegido
         * @param sol Conjunto solucion al problema
         * @return dis Suma de las distancia
         */
        double calcularSumaDistancias(int p,const vector<int> & sol);

        void setDistancia(double * _distancia,int n);
        void setRangoDistancia(pair<double,double> _distancia);
        void setCosteActual(double _coste);
        tFitness getCosteActual();
}
;
