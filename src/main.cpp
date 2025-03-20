#include <iostream>
#include <random.hpp>
#include <string>
#include <util.h>
// Real problem class
#include "pincrem.h"
#include "mindiff.h"
// All all algorithms
// #include "brutesearch.h"
// #include "greedy.h"
// #include "randomsearch.h"

using namespace std;

vector<string> archivos = {
  "datos/GKD-b_1_n25_m2.txt",
  "datos/GKD-b_2_n25_m2.txt",
  "datos/GKD-b_3_n25_m2.txt",
  "datos/GKD-b_4_n25_m2.txt",
  "datos/GKD-b_5_n25_m2.txt",
  "datos/GKD-b_6_n25_m7.txt",
  "datos/GKD-b_7_n25_m7.txt",
  "datos/GKD-b_8_n25_m7.txt",
  "datos/GKD-b_9_n25_m7.txt",
  "datos/GKD-b_10_n25_m7.txt",
  "datos/GKD-b_11_n50_m5.txt",
  "datos/GKD-b_12_n50_m5.txt",
  "datos/GKD-b_13_n50_m5.txt",
  "datos/GKD-b_14_n50_m5.txt",
  "datos/GKD-b_15_n50_m5.txt",
  "datos/GKD-b_16_n50_m15.txt",
  "datos/GKD-b_17_n50_m15.txt",
  "datos/GKD-b_18_n50_m15.txt",
  "datos/GKD-b_19_n50_m15.txt",
  "datos/GKD-b_20_n50_m15.txt",
  "datos/GKD-b_21_n100_m10.txt",
  "datos/GKD-b_22_n100_m10.txt",
  "datos/GKD-b_23_n100_m10.txt",
  "datos/GKD-b_24_n100_m10.txt",
  "datos/GKD-b_25_n100_m10.txt",
  "datos/GKD-b_26_n100_m30.txt",
  "datos/GKD-b_27_n100_m30.txt",
  "datos/GKD-b_28_n100_m30.txt",
  "datos/GKD-b_29_n100_m30.txt",
  "datos/GKD-b_30_n100_m30.txt",
  "datos/GKD-b_31_n125_m12.txt",
  "datos/GKD-b_32_n125_m12.txt",
  "datos/GKD-b_33_n125_m12.txt",
  "datos/GKD-b_34_n125_m12.txt",
  "datos/GKD-b_35_n125_m12.txt",
  "datos/GKD-b_36_n125_m37.txt",
  "datos/GKD-b_37_n125_m37.txt",
  "datos/GKD-b_38_n125_m37.txt",
  "datos/GKD-b_39_n125_m37.txt",
  "datos/GKD-b_40_n125_m37.txt",
  "datos/GKD-b_41_n150_m15.txt",
  "datos/GKD-b_42_n150_m15.txt",
  "datos/GKD-b_43_n150_m15.txt",
  "datos/GKD-b_44_n150_m15.txt",
  "datos/GKD-b_45_n150_m15.txt",
  "datos/GKD-b_46_n150_m45.txt",
  "datos/GKD-b_47_n150_m45.txt",
  "datos/GKD-b_48_n150_m45.txt",
  "datos/GKD-b_49_n150_m45.txt",
  "datos/GKD-b_50_n150_m45.txt"
};
int main(int argc, char *argv[]) {
  long int seed;

  // Seed the random number generator
  if (argc == 1) {
    // Use a fixed seed for reproducibility
    seed = 42;
  } else {
    seed = atoi(argv[1]);
  }

  // Create the algorithms
  //RandomSearch ralg = RandomSearch();
  //BruteSearch rbrute = BruteSearch();
  //GreedySearch rgreedy = GreedySearch();
  // Create the specific problem
  //ProblemIncrem rproblem = ProblemIncrem(10);
  
 
  // Solve using evaluations
  // vector<pair<string, MH *>> algoritmos = {make_pair("RandomSearch", &ralg),
  //                                          make_pair("BruteSearch", &rbrute),
  //                                          make_pair("Greedy", &rgreedy)};
  // Problem *problem = dynamic_cast<Problem *>(&rproblem);

  // for (int i = 0; i < algoritmos.size(); i++) {
  //   Random::seed(seed);
  //   cout << algoritmos[i].first << endl;
  //   MH *mh = algoritmos[i].second;
  //   ResultMH result = mh->optimize(problem, 100);
  //   cout << "Best solution: " << result.solution << endl;
  //   cout << "Best fitness: " << result.fitness << endl;
  //   cout << "Evaluations: " << result.evaluations << endl;
  // }
  cout << archivos[0] << endl;
  Mindiff problema(archivos[0]);
  tSolution solucion = problema.createSolution();
  for (const auto& elemento : solucion) {
    std::cout << elemento << " ";
}
  return 0;
}
