#include <stdio.h>
#include <stdlib.h>
#include "tabu_search.h"
#include "neighborhood.h"

s_tabu_list * allocate_tabu_list(int max_size) {
  s_tabu_list *tabu_list;

  tabu_list = (s_tabu_list*)malloc(sizeof(s_tabu_list));
  tabu_list->list = (s_tabu_move*)malloc(sizeof(s_move)*max_size);
  tabu_list->size = 0;
  tabu_list->max_size = max_size;
  
  return tabu_list;
}

int is_move_allowed(s_move *move, s_tabu_list *tabu_list, int current_it) {
  for(int i = 0; i < tabu_list->size; i++) {
    if(tabu_list->list[i].mach == move->a->op->machine
       &&
       tabu_list->list[i].job_a == move->a->op->job
       &&
       tabu_list->list[i].job_b == move->b->op->job) {
      return 0;
    }
  }
  return 1;
}

int select_index_neighbour(int index_best_neighbour, s_neighborhood * neighborhood, int current_index) {
  if (index_best_neighbour < 0) return current_index;
  if (neighborhood->neighbors[current_index].eval < neighborhood->neighbors[index_best_neighbour].eval) return current_index;

  int random = rand() % 2;

  if (random == 0) return current_index;

  return index_best_neighbour;
}

void update_tabu_list(s_tabu_list *tabu_list, s_move move, int current_it, int min_tabu_tenure) {
  s_tabu_move tmp;

  for(int i = 0; i < tabu_list->size; i++) {
    if (tabu_list->list[i].last_tabu_it < current_it) {
      tmp = tabu_list->list[i];
      tabu_list->size--;
      tabu_list->list[i] = tabu_list->list[tabu_list->size];
      tabu_list->list[tabu_list->size] = tmp;
      i--;
    }
  }
  
  int pos;

  if (tabu_list->size < tabu_list->max_size) {
    pos = tabu_list->size;
    tabu_list->size++;
  } else pos = rand() % tabu_list->max_size;

  int tabu_tenure = min_tabu_tenure + (rand() % (tabu_list->max_size - min_tabu_tenure));

  tabu_list->list[pos].mach = move.a->op->machine;
  tabu_list->list[pos].job_a = move.b->op->job;
  tabu_list->list[pos].job_b = move.a->op->job;
  tabu_list->list[pos].last_tabu_it = current_it + tabu_tenure;
}

int check_aspiration_criteria(s_move *move, s_job_shop_solution *best) {
  if (move->eval >= best->makespan) return 0;
  s_job_shop_solution *clone = clone_job_shop_solution(best);
  move->eval = apply_and_evaluate_move(clone, move);
  if (move->eval < best->makespan) return 1;
  return 0;
}

int find_best_non_tabu_neighbour_index(s_job_shop_solution * sol, s_neighborhood * neighborhood,
                                       s_job_shop_solution * best, s_tabu_list * tabu_list, int current_it) {
  int best_neighbour_index = -1;

  for(int i = 0; i < neighborhood->total_neighbors; i++) {
    neighborhood->neighbors[i].eval = estimate_evaluation_neighbour(sol, &neighborhood->neighbors[i]);
    if ((check_aspiration_criteria(&neighborhood->neighbors[i], best)
         || is_move_allowed(&neighborhood->neighbors[i], tabu_list, current_it))
        && (best_neighbour_index == -1)
        || neighborhood->neighbors[i].eval <= neighborhood->neighbors[best_neighbour_index].eval) {
      best_neighbour_index = select_index_neighbour(best_neighbour_index, neighborhood, i);
    }
  }

  if (best_neighbour_index == -1) return rand() % neighborhood->total_neighbors;

  return best_neighbour_index;
}

s_job_shop_solution * tabu_search(s_job_shop_solution * initial_sol, int max_it, int max_size_tabu, int min_tabu_tenure) {
  s_job_shop_solution *best = clone_job_shop_solution(initial_sol);
  s_job_shop_solution *current = clone_job_shop_solution(initial_sol);

  s_neighborhood * neighborhood = allocate_neighborhood(current->inst->n * current->inst->m);
  s_tabu_list *tabu_list = allocate_tabu_list(max_size_tabu);
  int best_index, iteration = 0;
  
  while (iteration < max_it) {
    iteration++;

    generate_neighbors(current, neighborhood);
    best_index = find_best_non_tabu_neighbour_index(current, neighborhood, best, tabu_list, iteration);
    apply_and_evaluate_move(current, &neighborhood->neighbors[best_index]);
    update_tabu_list(tabu_list, neighborhood->neighbors[best_index], iteration, min_tabu_tenure);

    if (current->makespan < best->makespan) {
      copy_sol(current, best);
    }
  }
  return best;
}
