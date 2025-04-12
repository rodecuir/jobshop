#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "local_search.h"
#include "neighborhood.h"

s_job_shop_solution * local_search(s_job_shop_solution * initial_sol) {
  s_job_shop_solution *best = clone_job_shop_solution(initial_sol);
  s_job_shop_solution *current = clone_job_shop_solution(initial_sol);

  s_neighborhood * neighborhood =
    allocate_neighborhood(current->inst->n * current->inst->m);
  
  int best_index, improve = 1, iteration = 0;

   while (improve) {
    improve = 0;
    iteration++;

    generate_neighbors(current, neighborhood);
    best_index = get_index_best_neighbour(current, neighborhood);
    std::cout << "best index: " << best_index << '\n';
    apply_and_evaluate_move(current, &neighborhood->neighbors[best_index]);

    std::cout << "Iteración: " << iteration
              << ", best: " << best->makespan
              << ", estimate_best_neighbor: " << neighborhood->neighbors[best_index].eval
              << ", exact_best_neighbor: " << current->makespan << '\n';

    if (current->makespan < best->makespan) {
      improve = 1;
      copy_sol(current, best);
    }
  }

  return best;
}
