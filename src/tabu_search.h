#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "neighborhood.h"
#include "solution.h"


/**
 * Estructura con los datos de un movimiento.
 */
struct s_tabu_move {
  int mach, job_a, job_b, last_tabu_it;
};

/**
 * Estructura para almacenar los movimientos de la búsqueda tabú.
 */
struct s_tabu_list {
  s_tabu_move *list;
  int size, max_size;
};

// Permite inicializar una lista de la búsqueda tabú.
s_tabu_list * allocate_tabu_list(int max_size);

// Verifica si un movimiento se puede realizar
int is_move_allowed(s_move *move, s_tabu_list *tabu_list, int current_it);
// Ayuda a tomar al mejor vecino considerando el índica actual y una selección aleatoria
int select_index_neighbour(int index_best_neighbour, s_neighborhood * neighborhood, int current_index);

// Actualiza la lista tabú con un nuevo movimiento
void update_tabu_list(s_tabu_list *tabu_list, s_move move, int current_it, int min_tabu_tenure);

// Verifica si un movimiento cumple con los criterios de aspiración
int check_aspiration_criteria(s_move *move, s_job_shop_solution *best);

// Busca el mejor vecino que no se encuentre en la lista tabú
int find_best_non_tabu_neighbour_index(s_job_shop_solution * sol, s_neighborhood * neighborhood, s_job_shop_solution * best, s_tabu_list * tabu_list, int current_it);

// Función principal que usa las funciones auxiliares previas para implementar la búsqueda tabú
s_job_shop_solution * tabu_search(s_job_shop_solution * initial_sol, int max_it, int max_size_tabu, int min_tabu_tenure);

#endif
