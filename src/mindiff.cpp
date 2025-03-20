#include <mindiff.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

    
    Mindiff::Mindiff(const string & path){
        ifstream archivo(path);
        if(!archivo){
            cerr << "Error al abrir el archivo" << endl;
        }

        archivo >> n;
        archivo >> m;

        matriz = new double*[n];
        for(int i = 0; i < n; i++){
            matriz[i] = new double[n];
        }
        
        int f;
        int c;
        double distancia;
        for(int i = 0; i < n; i++){
            for(int j = i; j < n; j++){
                archivo >> f;
                archivo >> c;
                archivo >> distancia;
                matriz[f][c] = distancia;
            }
        }

        archivo.close();
    }

    double Mindiff::calcularSumaDistancias(int p,const vector<int> & sol){
        double dis = 0;
        int u;
        for(int i = 0; i < sol.size();i++){
            u = sol[i];
            if(i != p){
                if(i < p)
                    dis += matriz[u][p];
                else
                    dis += matriz[p][u];
            }
        }
        return dis;
    }

    tFitness Mindiff::fitness(const tSolution &solution){
       double dis_max,dis_min,dis;
       distancia.first = 0;
       distancia.second = 0;
       dis = 0;
       puntos = {};
        for(int i = 0; i < solution.size();i++){
            if(solution[i])
                puntos.push_back(i);
        }
        for(int i = 0; i < puntos.size();i++){
            dis = 0;
            dis = calcularSumaDistancias(puntos[i],puntos);
            if (dis > dis_max)
                distancia.first = dis;
            if (dis < dis_min)
                distancia.second = dis;
        }
        return dis_max - dis_min;
    }

    tFitness Mindiff::fitness(const tSolution &solution,SolutionFactoringInfo *solution_info,unsigned pos_change, tDomain new_value){

    }

    tSolution Mindiff::createSolution(){

    }

    size_t Mindiff::getSolutionSize(const tSolution &solution){
        return m;
    }

    pair<tDomain, tDomain> Mindiff::getSolutionDomainRange(){
        
    }

    void Mindiff::imprimirMatriz() {
        if (!matriz) {
            cerr << "La matriz no ha sido inicializada." << endl;
            return;
        }
    
        cout << "Matriz de tamaño " << n << "x" << n << ":" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << matriz[i][j] << "\t";  // Tabulación para mejor visualización
            }
            cout << endl;
        }
    }