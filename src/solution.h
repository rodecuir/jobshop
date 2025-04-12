#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "./jssp.h"

/**
 * Estructura para representar la planificación de una operación.
 */
struct s_operation_schedule {
  s_operation *op; // Datos de la operación
  int seq_m; // Índice de la operación en la secuencia de la máquina actual
  int r; // Fecha más temprana de inicio de la operación (release date)
  int q; // Longitud del camino más largo de la operación actual al vértice N+1 (length of the tail)
  int t; // Longitud del camino más largo hasta el vértice N+1 (length of the tail)
};

/**
 * Estructura para representar una solución (factible).
 */
struct s_job_shop_solution {
  s_jssp *inst; // Datos del ejemplar
  s_operation_schedule **machs; // Matriz con permutaciones de las operaciones en las máquinas
  s_operation_schedule **ops; // Lista de operaciones planificadas, indexadas por id
  int makespan; // Tiempo que transcurre desde el inicio de la planificación hasta que esta termina
};

// Devuelve la operación previa (si existe) en la secuencia de la máquina actual
s_operation_schedule * get_predecessor_machine(s_job_shop_solution *sol, s_operation_schedule *op);
// Devuelve la operación siguiente (si existe) en la secuencia de la máquina actual
s_operation_schedule * get_successor_machine(s_job_shop_solution *sol, s_operation_schedule *op);
// Devuelve la operación previa (si existe) en la secuencia de trabajos actual
s_operation_schedule * get_predecessor_job(s_job_shop_solution *sol, s_operation_schedule *op);
// Devuelve la operación siguiente (si existe) en la secuencia de trabajos actual
s_operation_schedule * get_successor_job(s_job_shop_solution *sol, s_operation_schedule *op);

// Genera una solución factible
s_job_shop_solution *make_feasible_solution(s_jssp *inst);
// Inicializa los parámetros de una solución
s_job_shop_solution *allocate_sol(s_jssp *inst);
// Devuelve una copia de una solución
s_job_shop_solution *clone_job_shop_solution(s_job_shop_solution *src);

// Copia una solución de una estructura a otra
void copy_sol(s_job_shop_solution *src, s_job_shop_solution *dst);
// Imprime la planificación de una operación
void print_operation_schedule(s_operation_schedule *op);
// Imprime la solución generada
void print_job_shop_solution(s_job_shop_solution *sol);
// Imprime la planificación dada una solución
void print_schedule(s_job_shop_solution *sol);
// Calcula las fechas más tempranas de inicio de cada operación
void release_dates(s_job_shop_solution *sol);
// Calcula la longitud del camino más largo desde la operación actual al vértice N+1 (para cada operación)
void tail_lengths(s_job_shop_solution *sol);

// Ejecuta release_date, tail_length y regresa el makespan
int eval_solution(s_job_shop_solution *sol);

#endif
