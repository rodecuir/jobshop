#include <iostream>
#include <stdlib.h>

#include "jssp.h"
#include "solution.h"
#include "local_search.h"
#include "neighborhood.h"
#include "tabu_search.h"

// Una pregunta que se repite varias veces en el menú
void opciones_de_instancia() {
  std::cout << "¿Qué instancia te gustaría probar?\n\n";
  std::cout << "Instancias ABZ ----------------------------------------------------\n";
  std::cout << "abz5 / abz6 / abz7 / abz8 / abz9\n\n";
  std::cout << "Instancias FT -----------------------------------------------------\n";
  std::cout << "ft06 / ft10 / ft20\n\n";
  std::cout << "Instancias LA -----------------------------------------------------\n";
  std::cout << "la01 / la02 / la03 / la04 / la05 / la06 / la07 / la08 / la09 / la10\n";
  std::cout << "la11 / la12 / la13 / la14 / la15 / la16 / la17 / la18 / la19 / la20\n";
  std::cout << "la21 / la22 / la23 / la24 / la25 / la26 / la27 / la28 / la29 / la30\n";
  std::cout << "la31 / la32 / la33 / la34 / la35 / la36 / la37 / la38 / la39 / la40\n\n";
  std::cout << "Instancias ORB ----------------------------------------------------\n";
  std::cout << "orb01 / orb02 / orb03 / orb04 / orb05 / orb06 / orb07 / orb08\n";
  std::cout << "orb09 / orb10\n\n";
  std::cout << "Instancias SWV ----------------------------------------------------\n";
  std::cout << "swv01 / swv02 / swv03 / swv04 / swv05 / swv06 / swv07 / swv08\n";
  std::cout << "swv09 / swv10 / swv11 / swv12 / swv13 / swv14 / swv15 / swv16\n";
  std::cout << "swv17 / swv18 / swv19 / swv20\n\n";
  std::cout << "Instancias TA -----------------------------------------------------\n";
  std::cout << "ta01 / ta02 / ta03 / ta04 / ta05 / ta06 / ta07 / ta08 / ta09 / ta10\n";
  std::cout << "ta11 / ta12 / ta13 / ta14 / ta15 / ta16 / ta17 / ta18 / ta19 / ta20\n";
  std::cout << "ta21 / ta22 / ta23 / ta24 / ta25 / ta26 / ta27 / ta28 / ta29 / ta30\n";
  std::cout << "ta31 / ta32 / ta33 / ta34 / ta35 / ta36 / ta37 / ta38 / ta39 / ta40\n";
  std::cout << "ta41 / ta42 / ta43 / ta44 / ta45 / ta46 / ta47 / ta48 / ta49 / ta50\n";
  std::cout << "ta51 / ta52 / ta53 / ta54 / ta55 / ta56 / ta57 / ta58 / ta59 / ta60\n";
  std::cout << "ta61 / ta62 / ta63 / ta64 / ta65 / ta66 / ta67 / ta68 / ta69 / ta70\n";
  std::cout << "ta71 / ta72 / ta73 / ta74 / ta75 / ta76 / ta77 / ta78 / ta79 / ta80\n\n";
  std::cout << "Instancias YN -----------------------------------------------------\n";
  std::cout << "yn1 / yn2 / yn3 / yn4\n\n";
  std::cout << "Ingresa una opción: ";
}

/**
 * Menú interactivo para probar las funciones implementadas
 */
int main()
{
  int selection;
  s_jssp *instance = NULL;
  std::string filename;
  std::string full_path;
  s_job_shop_solution *sol = NULL;
  s_job_shop_solution *local_opt = NULL;
  int max_iter, max_size_tabu, min_tabu_tenure;
  
  do {
    std::cout << "---------- Menú ----------\n";
    std::cout << "[1] - Probar la lectura de una instancia\n";
    std::cout << "[2] - Probar la evaluación de soluciones\n";
    std::cout << "[3] - Probar el cálculo de la lista de vecinos\n";
    std::cout << "[4] - Probar la aplicación de una búsqueda aleatoria\n";    
    std::cout << "[5] - Probar la aplicación de una metaheurística de trayectoria\n";
    std::cout << "[6] - Generar un reporte con la comparación justa entre la búsqueda aleatoria y el método de trayectoria\n";
    std::cout << "[0] - Salir\n";
    std::cout << "Ingresa una opción: ";

    std::cin >> selection;

    std::cout << '\n';

    switch (selection) {
    case 1:
      opciones_de_instancia();      
      std::cin >> filename;
      std::cout << '\n';
      full_path = "instancias/" + filename;
      instance = read_instance(full_path.c_str());
      if (instance != NULL) print_instance(instance);
      else std::cout << "(No se encontró el archivo)\n";
      std::cout << '\n';
      break;
    case 2:
      opciones_de_instancia();
      std::cin >> filename;
      std::cout << '\n';
      full_path = "instancias/" + filename;
      instance = read_instance(full_path.c_str());            
      if (instance != NULL) {
        unsigned int seed = 1;
        srand(seed);
        std::cout << "SEED: " << seed << '\n';

        sol = make_feasible_solution(instance);
        std::cout << "\nSolución generada:\n";
        print_job_shop_solution(sol);

        std::cout << "\nCalculando realease dates...\n";
        release_dates(sol);

        std::cout << "\nCalculando tail lengths...\n";        
        tail_lengths(sol);

        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);
       
        std::cout << "\nMAKESPAN: " << sol->makespan << '\n';     
      }
      else std::cout << "(No se encontró el archivo)\n";
      std::cout << '\n';            
      break;
    case 3:
      opciones_de_instancia();
      std::cin >> filename;
      std::cout << '\n';
      full_path = "instancias/" + filename;
      instance = read_instance(full_path.c_str());
      if (instance != NULL) {
        unsigned int seed = 1;
        srand(seed);
        std::cout << "SEED: " << seed << '\n';

        sol = make_feasible_solution(instance);
        std::cout << "\nSolución generada:\n";
        print_job_shop_solution(sol);
        eval_solution(sol);
        
        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);
       
        std::cout << "\nMAKESPAN: " << sol->makespan << '\n';

        s_neighborhood * neighborhood =
          allocate_neighborhood(sol->inst->n * sol->inst->m);
        
        generate_neighbors(sol, neighborhood);

        int best_index = get_index_best_neighbour(sol, neighborhood);

        std::cout << "\nMejor vecino: "
                  << '[' << best_index << ']'
                  << " = (M" << neighborhood->neighbors[best_index].a->op->machine
                  << ", (" << neighborhood->neighbors[best_index].a->seq_m
                  << ") J" << neighborhood->neighbors[best_index].a->op->job
                  << "[" << neighborhood->neighbors[best_index].a->op->seq_j
                  << "], (" << neighborhood->neighbors[best_index].b->seq_m
                  << ") J" << neighborhood->neighbors[best_index].b->op->job
                  << "[" << neighborhood->neighbors[best_index].b->op->seq_j
                  << "]), eval = " << neighborhood->neighbors[best_index].eval
                  << '\n';
        
        apply_and_evaluate_move(sol, &neighborhood->neighbors[best_index]);

        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);
       
        std::cout << "\nMAKESPAN: " << sol->makespan << '\n';        
      }
      else std::cout << "(No se encontró el archivo)\n";
      std::cout << '\n';      
      break;
    case 4:
      opciones_de_instancia();
      std::cin >> filename;
      std::cout << '\n';
      full_path = "instancias/" + filename;
      instance = read_instance(full_path.c_str());
      if (instance != NULL) {
        unsigned int seed = 1;
        srand(seed);
        std::cout << "SEED: " << seed << '\n';

        sol = make_feasible_solution(instance);
        std::cout << "\nSolución generada:\n";
        print_job_shop_solution(sol);
        eval_solution(sol);
        
        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);
       
        std::cout << "\nMAKESPAN: " << sol->makespan << '\n';

        local_opt = local_search(sol);
        std::cout << "\nSolución optimizada con la búsqueda local:\n";
        print_job_shop_solution(local_opt);
        std::cout << "\nEvaluando solución optimizada con la búsqueda local...\n";
        eval_solution(local_opt);

        std::cout << "\nMAKESPAN: " << local_opt->makespan << '\n';

        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);
      }
      else std::cout << "(No se encontró el archivo)\n";
      std::cout << '\n';
      break;
    case 5:
      opciones_de_instancia();
      std::cin >> filename;
      std::cout << '\n';
      full_path = "instancias/" + filename;
      instance = read_instance(full_path.c_str());
      if (instance != NULL) {
        unsigned int seed = (unsigned int) time(NULL);
        srand(seed);
        std::cout << "SEED: " << seed << '\n';

        sol = make_feasible_solution(instance);
        std::cout << "\nSolución generada:\n";
        print_job_shop_solution(sol);
        eval_solution(sol);
        
        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);
       
        std::cout << "\nMAKESPAN: " << sol->makespan << '\n';

        std::cout << "Ingresa el número máximo de iteraciones (ej. 1000): ";
        std::cin >> max_iter;

        std::cout << "Ingresa el tamaño máximo de la lista tabú (ej. 8): ";
        std::cin >> max_size_tabu;

        std::cout << "Ingresa el tiempo mínimo que un movimiento permanece en la lista tabú (tenencia) (ej. 1): ";
        std::cin >> min_tabu_tenure;

        local_opt = tabu_search(sol, max_iter, max_size_tabu, min_tabu_tenure);

        std::cout << "\nSolución optimizada con la búsqueda tabú:\n";
        print_job_shop_solution(local_opt);
        
        std::cout << "\nEvaluando solución optimizada con la búsqueda tabú...\n";
        eval_solution(local_opt);
        
        std::cout << "\nMAKESPAN: " << local_opt->makespan << '\n';
        
        std::cout << "\nPlanificación generada:\n";
        print_schedule(sol);        
      }
      else std::cout << "(No se encontró el archivo)\n";
      std::cout << '\n';      
      break;
    case 6:
      opciones_de_instancia();
      std::cin >> filename;
      std::cout << '\n';
      full_path = "instancias/" + filename;
      instance = read_instance(full_path.c_str());
      if (instance != NULL) {
        unsigned int seed = (unsigned int) time(NULL);
        srand(seed);
        std::cout << "SEED: " << seed << '\n';

        sol = make_feasible_solution(instance);
        std::cout << "\nSolución generada con búsqueda aleatoria:\n";
        print_job_shop_solution(sol);
        eval_solution(sol);
        std::cout << "\nMAKESPAN (Búsqueda Aleatoria): " << sol->makespan << '\n';

        std::cout << "Ingresa el número máximo de iteraciones (ej. 1000): ";
        std::cin >> max_iter;

        std::cout << "Ingresa el tamaño máximo de la lista tabú (ej. 8): ";
        std::cin >> max_size_tabu;

        std::cout << "Ingresa el tiempo mínimo que un movimiento permanece en la lista tabú (tenencia) (ej. 1): ";
        std::cin >> min_tabu_tenure;

        local_opt = tabu_search(sol, max_iter, max_size_tabu, min_tabu_tenure);
        std::cout << "\nSolución optimizada con búsqueda tabú:\n";
        print_job_shop_solution(local_opt);
        eval_solution(local_opt);
        std::cout << "\nMAKESPAN (Búsqueda Tabú): " << local_opt->makespan << '\n';

        std::cout << "\n---------- Reporte de Comparación ----------\n";
        std::cout << "Método de Búsqueda Aleatoria:\n";
        std::cout << "MAKESPAN: " << sol->makespan << '\n';
        std::cout << "Método de Búsqueda Tabú:\n";
        std::cout << "MAKESPAN: " << local_opt->makespan << '\n';
        std::cout << "---------------------------------------------\n";
      } else {
        std::cout << "(No se encontró el archivo)\n";
      }
      std::cout << '\n';
      break;
    case 0:
      std::cout << "Saliendo del programa...\n";
      break;
    default:
      std::cout << "Opción inválida. Prueba de nuevo.'\n'";
      break;
    }
  } while (selection != 0);
  return 0;
}
