#include <iostream>
#include <algorithm>
#include <climits>
#include "solution.h"

s_operation_schedule * get_predecessor_machine(s_job_shop_solution *sol, s_operation_schedule *op) {
  if (op != NULL && op->seq_m > 0) return &sol->machs[op->op->machine][op->seq_m-1];
  return NULL;
}

s_operation_schedule * get_successor_machine(s_job_shop_solution *sol, s_operation_schedule *op) {
  if (op != NULL && op->seq_m < sol->inst->n - 1) return &sol->machs[op->op->machine][op->seq_m+1];
  return NULL;
}

s_operation_schedule * get_predecessor_job(s_job_shop_solution *sol, s_operation_schedule *op) {
  if (op != NULL && op->op->seq_j > 0) return sol->ops[ op->op->id - 1];
  return NULL;
}

s_operation_schedule * get_successor_job(s_job_shop_solution *sol, s_operation_schedule *op) {
  if (op != NULL && op->op->seq_j < sol->inst->m - 1) return sol->ops[op->op->id + 1];
  return NULL;
}

s_job_shop_solution *allocate_sol(s_jssp *inst) {
    s_job_shop_solution *sol = NULL;
    sol = (s_job_shop_solution *)malloc(sizeof(s_job_shop_solution));
    sol->inst = inst;
    sol->machs = (s_operation_schedule **)malloc(sizeof(s_operation_schedule *)*inst->m);
    for(int i =0; i < inst->m; i++) {
      sol->machs[i] = (s_operation_schedule *)malloc(sizeof(s_operation_schedule)*inst->n);
    }
    sol->ops = (s_operation_schedule **)malloc(sizeof(s_operation_schedule *)*(inst->n*inst->m));
    sol->makespan = INT_MAX;

    return sol;
}

s_job_shop_solution * clone_job_shop_solution(s_job_shop_solution *src) {
  s_job_shop_solution *dst = allocate_sol(src->inst);
  copy_sol(src, dst);
  return dst;
}

void copy_sol(s_job_shop_solution *src, s_job_shop_solution *dst) {
  dst->inst = src->inst;
  for (int i = 0; i < src->inst->m; i++) {
    for (int j = 0; j < src->inst->n; j++) {
      dst->machs[i][j].op = src->machs[i][j].op;
      dst->machs[i][j].seq_m = src->machs[i][j].seq_m;
      dst->machs[i][j].r = src->machs[i][j].r;
      dst->machs[i][j].t = src->machs[i][j].t;
      dst->machs[i][j].q = src->machs[i][j].q;
      dst->ops[dst->machs[i][j].op->id] = &dst->machs[i][j];
    }
  }
  dst->makespan = src->makespan;  
}

s_job_shop_solution *make_feasible_solution(s_jssp *inst) {
  s_job_shop_solution *sol = NULL;
  sol = allocate_sol(inst);
  
  // Se genera un arreglo para almacenar el número de operaciones que lleva cada máquina
  int machs_free_position[inst->m];
  for (int i = 0; i < inst->m; i++) machs_free_position[i] = 0; // Se inicializa con 0's

  // Se genera un arreglo para almacenar operaciones que no han sido programadas
  s_operation * availables[inst->n];
  // Asignamos las operaciones iniciales para cada trabajo
  for (int i =0; i < inst->n; i++) availables[i] = &inst->jobs[i][0];
  int total_availables = inst->n;
    
  int tmp_random, current_free_position_mach, k = 0;
  s_operation * op_random;

  // Mientras sigan habiendo operaciones sin planificar...
  while (total_availables > 0) {
    // 1. Se elige una operacion aleatoria de entre las que haya disponibles:
    tmp_random = rand() % total_availables;
    op_random = availables[tmp_random];

    // 2. Se planifica la operación...
    current_free_position_mach = machs_free_position[op_random->machine];
    sol->machs[op_random->machine][current_free_position_mach].op = op_random;
    sol->machs[op_random->machine][current_free_position_mach].seq_m = current_free_position_mach;
    sol->machs[op_random->machine][current_free_position_mach].r = -1;
    sol->machs[op_random->machine][current_free_position_mach].q = -1;
    sol->machs[op_random->machine][current_free_position_mach].t = -1;
    sol->ops[op_random->id] = &sol->machs[op_random->machine][current_free_position_mach];

    machs_free_position[op_random->machine]++; // Se aumenta el contador de op. de la máq. actual

    // 3. Si se tiene una operacion siguiente se reempla por la actual, de otro modo se elimina la
    // actual y se actualiza el contador de operaciones por planificar.
    if (op_random->seq_j < inst->m-1) {
      availables[tmp_random] =  &inst->jobs[op_random->job][op_random->seq_j+1]; // Se obtiene la siguiente op.
    } else {
      availables[tmp_random] = availables[total_availables - 1];
      total_availables--;
    }
  }

  return sol;  
}

void print_operation_schedule(s_operation_schedule *op) {
  if (op != NULL) {
    std::cout << "( J" << op->op->job
              << "[" << op->op->seq_j << "], M"
              << op->op->machine << "[" << op->seq_m << "], r="
              << op->r << ", d=" << op->op->time << ")\n";
  } else {
    std::cout << " - \n";
  }  
}

void print_job_shop_solution(s_job_shop_solution *sol) {
  for (int i = 0; i < sol->inst->m; i++) {
    std::cout << "M " << i << " [ ";
    for (int j = 0; j < sol->inst->n; j++) {
      std::cout << sol->machs[i][j].op->job << ", ";
    }
    std::cout << "]\n";
  }  
}

void print_schedule(s_job_shop_solution *sol) {
  for (int i =0; i < sol->inst->m; i++) {
    std::cout << "M " << i << " [ ";
    for (int j=0; j < sol->inst->n; j++) {
      std::cout << "(J" << sol->machs[i][j].op->job 
                << "[" << sol->machs[i][j].op->seq_j
                << "] r=" << sol->machs[i][j].r 
                << ", d=" << sol->machs[i][j].op->time 
                << ", t=" << sol->machs[i][j].t
                << "), ";      
    }
    std::cout << "]\n";
  }  
}

void release_dates(s_job_shop_solution *sol) {
  s_operation_schedule * availables[sol->inst->n + 1];
  int total_availables = 0;

  s_operation *job_op_current;
  s_operation_schedule *mach_op_current, *predecessor_job, *predecessor_mach, *successor_job, *successor_mach;

  int labeled[sol->inst->n*sol->inst->n];
  for (int i=0; i < sol->inst->n*sol->inst->n; i++) labeled[i] = 0;

  sol->makespan = 0;

  // Calculamos las operaciones iniciales de cada trabajo:
  for (int i = 0; i < sol->inst->n; i++) {
    job_op_current = &sol->inst->jobs[i][0];
    mach_op_current = &sol->machs[job_op_current->machine][0];
    if (mach_op_current->op == job_op_current) {
      availables[total_availables] = mach_op_current;
      total_availables++;
    }
  }

  while (total_availables > 0) {
    total_availables--;
    mach_op_current = availables[total_availables];
    job_op_current = mach_op_current->op;

    // Calculamos r_i (release date de la operación i)
    predecessor_mach = get_predecessor_machine(sol, mach_op_current);
    predecessor_job = get_predecessor_job(sol, mach_op_current);

    int r_pred_mach = predecessor_mach != NULL ? predecessor_mach->r : 0;
    int r_pred_job = predecessor_job != NULL ? predecessor_job->r : 0;
    
    mach_op_current->r = std::max(
        r_pred_mach + (predecessor_mach != NULL ? predecessor_mach->op->time : 0),  
        r_pred_job + (predecessor_job != NULL ? predecessor_job->op->time : 0)
    );
    
    labeled[mach_op_current->op->id] = 1;

    sol->makespan = std::max(sol->makespan, mach_op_current->r + mach_op_current->op->time);

    successor_job = get_successor_job(sol, mach_op_current);

    // Verificamos si el siguiente trabajo en la secuencia del job puede ser planificado
    // para que el siguiente trabajo pueda ser planificado, el trabajo predecesor en la máquina debe estar planificado
    if (successor_job != NULL // existe el trabajo sucesor en el job
        && (get_predecessor_machine(sol, successor_job) == NULL // no tiene trabajo predecesor en la máquina del sucesor
            || labeled[get_predecessor_machine(sol, successor_job)->op->id])){  // el predecesor en la maquina ya fue calendarizado
      availables[total_availables] = successor_job;
      total_availables++;
    }

    successor_mach = get_successor_machine(sol, mach_op_current);

    // Verificamos si el siguiente trabajo en la secuencia de la maquina puede ser planificado
    // para que el siguiente pueda ser planificado, el trabajo predecesor del job estar planificado
    if (successor_mach != NULL // existe el trabajo sucesor en la maquina
        && (get_predecessor_job(sol, successor_mach) == NULL // no tiene trabajo predecesor en la máquina del sucesor
            || get_predecessor_job(sol, successor_mach)->r >= 0)) { // el predecesor en el job ya fue calendarizado
      availables[total_availables] = successor_mach;      
      total_availables++;
    }
  }
}

void tail_lengths(s_job_shop_solution *sol) {
  s_operation_schedule * availables[sol->inst->n + 1]; 
  int total_availables = 0;

  s_operation *job_op_current;
  s_operation_schedule *mach_op_current, *predecessor_job, *predecessor_mach, *successor_job, *successor_mach;
  
  sol->makespan = 0;

  int labeled[sol->inst->n*sol->inst->n];
  for (int i=0; i < sol->inst->n*sol->inst->n; i++) labeled[i] = 0;

  // Calculamos las operaciones finales de cada trabajo:
  for (int i =0; i < sol->inst->n; i++) {
    job_op_current = &sol->inst->jobs[i][sol->inst->m - 1];
    mach_op_current = &sol->machs[job_op_current->machine][sol->inst->n - 1];

    if (mach_op_current->op == job_op_current) {
      availables[total_availables] = mach_op_current;
      total_availables++;
    }
  }

  while (total_availables > 0) {
    total_availables--;
    mach_op_current = availables[total_availables];
        
    job_op_current = mach_op_current->op;

    // Calculamos q_i (longitud del camino más largo al último nodo)
    successor_mach = get_successor_machine(sol, mach_op_current);
    successor_job = get_successor_job(sol, mach_op_current);

    int q_suc_mac = successor_mach != NULL ? successor_mach->q : 0;
    int q_suc_job = successor_job != NULL ? successor_job->q : 0;

    mach_op_current->t = std::max( q_suc_mac,  q_suc_job);
    mach_op_current->q = mach_op_current->t + mach_op_current->op->time;
       
    labeled[mach_op_current->op->id] = 1;
        
    sol->makespan = std::max(sol->makespan, mach_op_current->q);

    predecessor_job = get_predecessor_job(sol, mach_op_current);

    // Verificamos si el predecesor en la secuencia del job puede ser calculado
    // para que el predecesor pueda ser calculado, el q del trabajo sucesor en la máquina debe estar calculado
    if (predecessor_job != NULL // existe el trabajo predecesor en el job
        && (get_successor_machine(sol, predecessor_job) == NULL // no tiene sucesor en la máquina del predecesor
            ||  labeled[get_successor_machine(sol, predecessor_job)->op->id])) {  // el sucesor en la maquina ya fue calculado
      availables[total_availables] = predecessor_job;
      total_availables++;
    }

    predecessor_mach = get_predecessor_machine(sol, mach_op_current);

    // Verificamos si el predecesor en la secuencia de la maquina puede ser calculado
    // para que el predecesor pueda ser planificado, el trabajo sucesor del job estar calculado
    if (predecessor_mach != NULL // existe el trabajo predecesor en la maquina
        && (!get_successor_job(sol, predecessor_mach) // no tiene trabajo sucesor en la máquina del precedesor
            || get_successor_job(sol, predecessor_mach)->q >= 0)) { // el sucesor en el job ya fue calendarizado
      availables[total_availables] = predecessor_mach;
      total_availables++;
    }
  }
}

int eval_solution(s_job_shop_solution *sol) {
  release_dates(sol);
  tail_lengths(sol);
  return sol->makespan;
}
