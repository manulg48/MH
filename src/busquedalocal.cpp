#include <cassert>
#include "mindiff.h"
#include "busquedalocal.h"
#include "random.hpp"
#include <algorithm>
using namespace std;
bool verifica(vector<tOption> & sel,tOption i, tOption j,int m){
    if(find(sel.begin(),sel.end(),j) != sel.end())
        return false;
    
    if(find(sel.begin(),sel.end(),i) == sel.end())
        return false;
    
    if(sel.size() != m)
        return false;
    
    vector<tOption> aux = sel;
    sort(aux.begin(),aux.end());
    if(adjacent_find(aux.begin(),aux.end()) != aux.end())
        return false;
    
    return true;
    

}

double calcularCoste(vector<tOption> & sel,tOption i, tOption j,double ** matriz){
    
}
ResultMH BusquedaLocal::optimize(Problem *problem, int maxevals){
    assert(maxevals == 100000);
    Mindiff *realproblem = dynamic_cast<Mindiff *>(problem);
    auto m = problem->getSolutionSize();
    auto n = problem->getProblemSize();
    double ** matriz = realproblem->getMatriz();
    tOption j;
    tSolution sol = problem->createSolution();
    vector<tOption> values(m);
    vector<tOption> vecinos;
    for(int i = 0; i < n; i++){
        if (sol[i])
            values.push_back(i);
    }

    for(int l = 0; l < n; l++){
        vecinos.push_back(l);
    }

    for(int i = 0; i < m;i++){
        vecinos.erase(remove(vecinos.begin(),vecinos.end(),values[i]),vecinos.end());
    }

    for(int i = 0; i < m; i++){
        do{
            do{
                j = Random::get<tOption>(0,n-1);
            }while(find(values.begin(),values.end(),j)!=values.end());


        }while (calcularCoste(values,i,j,matriz));
    }

    return ResultMH(sol, fitness,100000);
}