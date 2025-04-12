#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include "neighborhood.h"
#include "solution.h"

s_neighborhood * allocate_neighborhood(int max_neighbour) {
  s_neighborhood * neighborhood;
  neighborhood = (s_neighborhood*)malloc(sizeof(s_neighborhood));  
  neighborhood->neighbors = (s_move*)malloc(sizeof(s_move)*max_neighbour);
  neighborhood->total_neighbors = 0;
  return neighborhood;
}

int is_critical_operation(int mach, int seq_mach, s_job_shop_solution *sol) {
  return (sol->machs[mach][seq_mach].r + sol->machs[mach][seq_mach].q) == sol->makespan;
}

void generate_neighbors(s_job_shop_solution *sol, s_neighborhood * neighborhood) {
  neighborhood->total_neighbors = 0;
  for(int i = 0; i < sol->inst->m; i++) {
    for(int j = 0; j < sol->inst->n - 1; j++) {
      if ((sol->machs[i][j].r + sol->machs[i][j].q) == sol->makespan
          && (sol->machs[i][j+1].r + sol->machs[i][j+1].q) == sol->makespan) {
        neighborhood->neighbors[neighborhood->total_neighbors].a = &sol->machs[i][j];
        neighborhood->neighbors[neighborhood->total_neighbors].b = &sol->machs[i][j+1];
        neighborhood->total_neighbors++;
      }        
    }
  }
}

int is_bloq_ini(int mach, int seq_mach, s_job_shop_solution *sol) {
  return
    is_critical_operation(mach, seq_mach, sol)
    && is_critical_operation(mach, seq_mach + 1, sol)
    && (seq_mach == 0 || !is_critical_operation(mach, seq_mach - 1, sol));
}

int is_bloq_end(int mach, int seq_mach, s_job_shop_solution *sol) {
  return
    is_critical_operation(mach, seq_mach, sol)
    && (seq_mach > 0 && is_critical_operation(mach, seq_mach - 1, sol))
    && (seq_mach == sol->inst->n - 1 || !is_critical_operation(mach, seq_mach + 1, sol));
}

int estimate_evaluation_neighbour(s_job_shop_solution *sol, s_move * neighbour) {
  s_operation_schedule *predecessor_mach_a = get_predecessor_machine(sol, neighbour->a);
  s_operation_schedule *predecessor_job_a = get_predecessor_job(sol, neighbour->a);
  s_operation_schedule *predecessor_job_b = get_predecessor_job(sol, neighbour->b);
  s_operation_schedule *successor_job_a = get_successor_job(sol, neighbour->a);
  s_operation_schedule *successor_job_b = get_successor_job(sol, neighbour->b);
  s_operation_schedule *successor_mach_b = get_successor_job(sol, neighbour->b);

  int r_b_ = std::max(
    predecessor_mach_a != NULL ? (predecessor_mach_a->r + predecessor_mach_a->op->time) : 0,
    predecessor_job_b != NULL ? (predecessor_job_b->r + predecessor_job_b->op->time) : 0);

  int r_a_ = std::max(
    r_b_ + neighbour->b->op->time,
    predecessor_job_a != NULL ? (predecessor_job_a->r + predecessor_job_a->op->time) : 0);

  int q_a_ = std::max(
    successor_mach_b != NULL ? successor_mach_b->q : 0,
    successor_job_a != NULL ? successor_job_a->q : 0) + neighbour->a->op->time;

  int q_b_ = std::max(
    q_a_,
    successor_job_b != NULL ? successor_job_b->q : 0) + neighbour->b->op->time;
  
  neighbour->eval = std::max(r_b_ + q_b_, r_a_ + q_a_);

  return neighbour->eval; 
}

int get_index_best_neighbour(s_job_shop_solution *sol, s_neighborhood * neighborhood) {
  int best_index = 0;

  for (int i = 0; i < neighborhood->total_neighbors; i++) {
    neighborhood->neighbors[i].eval = estimate_evaluation_neighbour(sol, &neighborhood->neighbors[i]);
    if (neighborhood->neighbors[i].eval < neighborhood->neighbors[best_index].eval) best_index = i;
  }

  return best_index;
}

int apply_and_evaluate_move(s_job_shop_solution *sol, s_move *neighbour) {
  int mach = neighbour->a->op->machine;
  int pos_a = neighbour->a->seq_m;
  int pos_b = neighbour->b->seq_m;
  

  s_operation *tmp = sol->machs[mach][pos_a].op;
  sol->machs[mach][pos_a].op = sol->machs[mach][pos_b].op;
  sol->ops[sol->machs[mach][pos_a].op->id] = &sol->machs[mach][pos_a];

  sol->machs[mach][pos_b].op = tmp;
  sol->ops[sol->machs[mach][pos_b].op->id] = &sol->machs[mach][pos_b];
  int total_operations = sol->inst->n * sol->inst->m;

  for (int i = 0; i < total_operations; i++) {
    sol->ops[i]->r = -1;
    sol->ops[i]->q = -1;
    sol->ops[i]->t = -1;
  }

  sol->makespan = eval_solution(sol);

  return sol->makespan;    
}
