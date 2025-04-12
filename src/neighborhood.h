#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "solution.h"

/**
 * Estructura para almacenar los datos de los vecinos.
 */
struct s_move {
  s_operation_schedule *a;
  s_operation_schedule *b;

  int eval;
};

/**
 * Estructura para representar la vecindad.
 */
struct s_neighborhood {
  s_move * neighbors;
  
  int total_neighbors;
};

// Inicializa la vecindad
s_neighborhood * allocate_neighborhood(int max_neighbour);

// Verifica si una operación es crítica
int is_critical_operation(int mach, int seq_mach, s_job_shop_solution *sol);

// Genera vecinos para la solución
void generate_neighbors(s_job_shop_solution *sol, s_neighborhood * neighborhood);

// Verifica si una operación es un bloque al inicio
int is_bloq_ini(int mach, int seq_mach, s_job_shop_solution *sol);

// Verifica si una operación es un bloqueo al final
int is_bloq_end(int mach, int seq_mach, s_job_shop_solution *sol);

// Estima la evaluación de un vecino
int estimate_evaluation_neighbour(s_job_shop_solution *sol, s_move * neighbour);

// Regresa el índica del mejor vecino
int get_index_best_neighbour(s_job_shop_solution *sol, s_neighborhood * neighborhood);

// Aplica y evalúa un movimiento de los vecinos
int apply_and_evaluate_move(s_job_shop_solution *sol, s_move * neighbour);

#endif
