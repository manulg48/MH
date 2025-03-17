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
        int distancia;
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

    tFitness Mindiff::fitness(const tSolution &solution){

    }

    tFitness Mindiff::fitness(const tSolution &solution, unsigned pos_previous,
        tDomain new_value) {
        tSolution newsol(solution);
        newsol[pos_previous] = new_value;
        return fitness(newsol);
    }

    tSolution Mindiff::createSolution(){

    }

    size_t Mindiff::getSolutionSize(){

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