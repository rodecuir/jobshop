#include "jssp.h"
#include <fstream>
#include <iostream>
#include <vector>

s_jssp * read_instance(const char *filename)
{
  s_jssp *instance = new s_jssp;
  std::ifstream file(filename);
  
  if(file.is_open()) {
    file >> instance->n >> instance->m;
    instance->jobs.assign(instance->n, std::vector<s_operation>(instance->m));

    int k = 0;
    for (int i = 0; i < instance->n; i++) {
      for (int j = 0; j < instance->m; j++) {
        file >> instance->jobs[i][j].machine >> instance->jobs[i][j].time;
        instance->jobs[i][j].id = k++;
        instance->jobs[i][j].job = i;
        instance->jobs[i][j].seq_j = j;
      }
    }
      
    file.close();
    return instance;
  } else {
    std::cerr << "Couldn't find " << filename << '\n';
    return nullptr;
  }
}

void print_instance(s_jssp *instance) {
  std::cout << instance->n << " " << instance ->m <<'\n';

  for (int i = 0; i < instance->n; ++i) {
    for (int j = 0; j < instance->jobs[i].size(); ++j) {
      std::cout << instance->jobs[i][j].machine << " " << instance->jobs[i][j].time << " ";
    }
    std::cout << '\n';
  }
} 
