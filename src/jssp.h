#ifndef JSSP_H
#define JSSP_H

#include <vector>

/**
 * Estructura para representar las operaciones.
 */
struct s_operation {
  int id; // Índice de la operación
  int job; // Índice del trabajo que corresponde a esta operación
  int machine; // Índice de la máquina que corresponde a esta operación
  int time; // Tiempo de procesamiento de la operación
  int seq_j; // Índice de la operación en la secuencia del trabajo actual
};

/**
 * Estructura para representar el problema.
 */
struct s_jssp {
  int n; // Número de trabajos
  int m; // Número de máquinas

  std::vector<std::vector<s_operation>> jobs; // Matriz con las operaciones
};

s_jssp * read_instance(const char *filename);
void print_instance(s_jssp *instance);

#endif
