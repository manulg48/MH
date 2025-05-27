// === main.cpp completo ===
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <numeric>
#include <limits>

#include <random.hpp>
#include "util.h"
#include "mindiff.h"
#include "busquedalocal.h"
#include "BMB.h"
#include "ILS.h"
#include "greedy.h"
#include "memetic.h"
#include "GRASP_SiBL.h"
#include "GRASP_NoBL.h"
#include "EnfriamientoSimulado.h"
using namespace std;
using namespace chrono;

// Struct para acumular resultados
struct Resultado {
    double desv_total    = 0.0;
    double tiempo_total  = 0.0;
    double evals_total   = 0.0;  // nuevo acumulador de evaluaciones
    int    contador      = 0;

    // ahora recibe 3 parámetros
    void agregar(double desv, double tiempo, double evals) {
        desv_total   += desv;
        tiempo_total += tiempo;
        evals_total  += evals;
        contador++;
    }

    double media_desv() const { return desv_total   / contador; }
    double media_tiempo() const { return tiempo_total / contador; }
    double media_evals() const { return evals_total   / contador; }
};

// Extrae el valor de n del nombre de archivo
int extraer_n(const string& nombre) {
    size_t pos_n = nombre.find("_n");
    size_t pos_m = nombre.find("_m", pos_n);
    return stoi(nombre.substr(pos_n + 2, pos_m - pos_n - 2));
}

// Diccionario con los mejores valores conocidos
map<string, double> mejores_valores = {
    {"GKD-b_1_n25_m2.txt", 0.000000}, {"GKD-b_2_n25_m2.txt", 0.000000},
    {"GKD-b_3_n25_m2.txt", 0.000000}, {"GKD-b_4_n25_m2.txt", 0.000000},
    {"GKD-b_5_n25_m2.txt", 0.000000}, {"GKD-b_6_n25_m7.txt", 12.717960},
    {"GKD-b_7_n25_m7.txt", 14.098750}, {"GKD-b_8_n25_m7.txt", 16.761190},
    {"GKD-b_9_n25_m7.txt", 17.069210}, {"GKD-b_10_n25_m7.txt", 23.265230},
    {"GKD-b_11_n50_m5.txt", 1.926100}, {"GKD-b_12_n50_m5.txt", 2.121040},
    {"GKD-b_13_n50_m5.txt", 2.362310}, {"GKD-b_14_n50_m5.txt", 1.663200},
    {"GKD-b_15_n50_m5.txt", 2.853130}, {"GKD-b_16_n50_m15.txt", 42.745780},
    {"GKD-b_17_n50_m15.txt", 48.107610}, {"GKD-b_18_n50_m15.txt", 43.196090},
    {"GKD-b_19_n50_m15.txt", 46.412450}, {"GKD-b_20_n50_m15.txt", 47.715110},
    {"GKD-b_21_n100_m10.txt", 13.832020}, {"GKD-b_22_n100_m10.txt", 13.664340},
    {"GKD-b_23_n100_m10.txt", 15.345380}, {"GKD-b_24_n100_m10.txt", 8.640640},
    {"GKD-b_25_n100_m10.txt", 17.200510}, {"GKD-b_26_n100_m30.txt", 168.729590},
    {"GKD-b_27_n100_m30.txt", 127.097260}, {"GKD-b_28_n100_m30.txt", 106.379190},
    {"GKD-b_29_n100_m30.txt", 137.453160}, {"GKD-b_30_n100_m30.txt", 127.479740},
    {"GKD-b_31_n125_m12.txt", 11.745140}, {"GKD-b_32_n125_m12.txt", 18.788930},
    {"GKD-b_33_n125_m12.txt", 18.531600}, {"GKD-b_34_n125_m12.txt", 19.488330},
    {"GKD-b_35_n125_m12.txt", 18.112420}, {"GKD-b_36_n125_m37.txt", 155.434770},
    {"GKD-b_37_n125_m37.txt", 198.894620}, {"GKD-b_38_n125_m37.txt", 187.967030},
    {"GKD-b_39_n125_m37.txt", 168.590200}, {"GKD-b_40_n125_m37.txt", 178.193740},
    {"GKD-b_41_n150_m15.txt", 23.346080}, {"GKD-b_42_n150_m15.txt", 26.789500},
    {"GKD-b_43_n150_m15.txt", 26.754470}, {"GKD-b_44_n150_m15.txt", 25.935590},
    {"GKD-b_45_n150_m15.txt", 27.773010}, {"GKD-b_46_n150_m45.txt", 227.749310},
    {"GKD-b_47_n150_m45.txt", 228.602900}, {"GKD-b_48_n150_m45.txt", 226.745340},
    {"GKD-b_49_n150_m45.txt", 226.409610}, {"GKD-b_50_n150_m45.txt", 248.856620}
};

// Vector con los archivos a procesar
vector<string> archivos = [](){
    vector<string> a;
    for (int i = 1; i <= 50; ++i) {
        string path = "datos/GKD-b_" + to_string(i) + "_n";
        if (i <= 5) path += "25_m2.txt";
        else if (i <= 10) path += "25_m7.txt";
        else if (i <= 15) path += "50_m5.txt";
        else if (i <= 20) path += "50_m15.txt";
        else if (i <= 25) path += "100_m10.txt";
        else if (i <= 30) path += "100_m30.txt";
        else if (i <= 35) path += "125_m12.txt";
        else if (i <= 40) path += "125_m37.txt";
        else if (i <= 45) path += "150_m15.txt";
        else path += "150_m45.txt";
        a.push_back(path);
    }
    return a;
}();

int main(int argc, char *argv[]) {
    long int seed = (argc == 2) ? atoi(argv[1]) : 42;

    map<string, map<string, Resultado>> tabla1;
    map<string, map<int, Resultado>>     tabla2;
    map<string, Resultado>               tabla3;

    vector<pair<string, MH *>> algoritmos = {
        {"Greedy",      new GreedySearch()},
        {"LSrandom",    new BusquedaLocal()},
        {"Mejor Práctica 2", new AM101},
        {"BMB",         new BMB()},
        {"ILS",         new ILS()},
        {"GRASP-SiBL",  new GRASPSiBL()},
        {"GRASP-NoBL", new GRASPNoBL()},
        {"ES", new EnfriamientoSimulado()}
    };

    for (size_t i = 0; i < archivos.size(); ++i) {
        const auto& archivo = archivos[i];
        string nombre_archivo = archivo.substr(archivo.find_last_of("/") + 1);
        if (mejores_valores.find(nombre_archivo) == mejores_valores.end())
            continue;

        cout << "\n>>> Procesando caso " << i+1 << "/"
             << archivos.size() << ": " << nombre_archivo << endl;

        Mindiff problema(archivo);
        double valor_optimo = mejores_valores[nombre_archivo];
        int n = extraer_n(nombre_archivo);

        for (auto& [nombre_algoritmo, algoritmo] : algoritmos) {
            int repeticiones = (nombre_algoritmo == "Greedy") ? 1 : 5;
            vector<double> desv, tiempos, iteraciones;

            cout << "    -> Algoritmo: " << nombre_algoritmo
                 << " (" << repeticiones << " rep)" << endl;

            for (int r = 0; r < repeticiones; ++r) {
                cout << "       --> Ejecución " << r+1 << "... " << flush;
                Random::seed(seed + r);
                auto start = high_resolution_clock::now();
                ResultMH result = algoritmo->optimize(
                    &problema,
                    (nombre_algoritmo == "Greedy") ? 1 : 100000
                );
                auto end = high_resolution_clock::now();

                double tiempo = duration_cast<milliseconds>(end - start).count();
                double desviacion;
                if (result.fitness == 0.0) {
                    desviacion = (valor_optimo == 0.0) ? 0.0 : 100.0;
                } else {
                    desviacion = 100.0 * (result.fitness - valor_optimo)
                                      / result.fitness;
                }

                desv.push_back(desviacion);
                tiempos.push_back(tiempo);
                iteraciones.push_back(result.evaluations);

                cout << "OK (fit=" << result.fitness
                     << ", t=" << tiempo << "ms"
                     << ", desv=" << fixed<<setprecision(2)
                     << desviacion << "%, evals=" << result.evaluations
                     << ")\n";
            }

            // calcular medias
            double md = accumulate(desv.begin(), desv.end(), 0.0) /
                        desv.size();
            double mt = accumulate(tiempos.begin(), tiempos.end(), 0.0) /
                        tiempos.size();
            double me = accumulate(iteraciones.begin(), iteraciones.end(), 0.0) /
                        iteraciones.size();

            // agregar a las 3 tablas
            tabla1[nombre_algoritmo][nombre_archivo].agregar(md, mt, me);
            tabla2[nombre_algoritmo][n].agregar(md, mt, me);
            tabla3[nombre_algoritmo].agregar(md, mt, me);

            cout << "    -> Resultado medio: desv=" << fixed<<setprecision(2)
                 << md << "%, t=" << mt << "ms, evals=" << me << "\n";
        }
    }

    // Volcado tabla1
    ofstream f1("tabla1_resultados_por_caso.csv");
    f1 << "Algoritmo,Caso,Desv,Tiempo(ms),Evals\n";
    for (const auto& [alg, casos] : tabla1) {
        vector<pair<string, Resultado>> ord(casos.begin(), casos.end());
        sort(ord.begin(), ord.end(),
             [](auto &a, auto &b){ return a.first < b.first; });
        for (auto& [caso, r] : ord) {
            f1 << alg << "," << caso << ","
               << fixed<<setprecision(2)<< r.media_desv()   << ","
               <<             r.media_tiempo() << ","
               <<             static_cast<int>(std::round(r.media_evals()))  << "\n";
        }
    }
    f1.close();

    // Volcado tabla2
    ofstream f2("tabla2_resultados_por_tamano.csv");
    f2 << "Algoritmo,Tamano,Desv,Tiempo(ms),Evals\n";
    for (auto& [alg, tamanos] : tabla2) {
        for (auto& [tam, r] : tamanos) {
            f2 << alg << "," << tam << ","
               << fixed<<setprecision(2)<< r.media_desv()   << ","
               <<             r.media_tiempo() << ","
               <<             static_cast<int>(std::round(r.media_evals()))  << "\n";
        }
    }
    f2.close();

    // Volcado tabla3
    ofstream f3("tabla3_resultados_globales.csv");
    f3 << "Algoritmo,Desv,Tiempo(ms),Evals\n";
    for (auto& [alg, r] : tabla3) {
        f3 << alg << ","
           << fixed<<setprecision(2)<< r.media_desv()   << ","
           <<             r.media_tiempo() << ","
           <<             static_cast<int>(std::round(r.media_evals())) << "\n";
    }
    f3.close();

    cout << "✅ Tablas generadas correctamente." << endl;
    return 0;
}
