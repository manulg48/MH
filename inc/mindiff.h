#include "problem.h"

using namespace std;

class Mindiff : public Problem{
    private:
        int n;
        int m;
        vector<int> puntos;
        pair<double,double> distancia;


        double **matriz;
    public:
        Mindiff(const string& path);
        ~Mindiff(){};
        /***
        * Evaluate the solution from scratch.
        * @param solution to evaluate.
        */
       tFitness fitness(const tSolution &solution) = 0;
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
        size_t getSolutionSize(const tSolution &solution);
        /** Return the range of domain of each element of the solution */
        pair<tDomain, tDomain> getSolutionDomainRange() override;
    
        //Imprime la matriz
        void imprimirMatriz();

        /**
         * Calcula la suma de las distancioas de un punto elegido al resto
         * @param p punto elegido
         * @param sol Conjunto solucion al problema
         * @return dis Suma de las distancia
         */
        double calcularSumaDistancias(int p,const vector<int> & sol);

}
;
