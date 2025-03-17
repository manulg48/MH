#include "problem.h"

using namespace std;

class Mindiff : public Problem{
    private:
        int n;
        int m;

        double **matriz;
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
         * @param pos_previous position of the solution to change.
         * @param new_value.
         */
        tFitness fitness(const tSolution &solution, unsigned pos_previous,tDomain new_value) override;
        /**
        * Create a new solution.
        */
        tSolution createSolution() override;
        /**
        * Return the current size of the solution.
        */
        size_t getSolutionSize() override;
        /** Return the range of domain of each element of the solution */
        pair<tDomain, tDomain> getSolutionDomainRange() override;
    
        //Imprime la matriz
        void imprimirMatriz();
    

}
;
