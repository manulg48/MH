#include <problem.h>

using namespace std;

class Mindiff : public Problem{
    public:
        Mindiff();
        
        tFitness fitness(const tSolution &solution);

        tFitness fitness(const tSolution &solution, unsigned pos_previous,tDomain new_value);

        tSolution createSolution();
        
        size_t getSolutionSize();
        
        pair<tDomain, tDomain> getSolutionDomainRange();
    
    private:
        
}
;
