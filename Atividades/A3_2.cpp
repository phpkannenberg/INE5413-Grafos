#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Wrong format." << std::endl;
        return EXIT_FAILURE;
    }
    
    std::ifstream is(argv[1]);
    if (!is)
    {
        std::cerr << "File error." << std::endl;
        return EXIT_FAILURE;
    }
    
    auto g(le_grafo(is));
    auto emp_max(g.algoritmo_hopcroft_karp());
    
    std::cout << emp_max.m << '\n';
    
    auto emp_impressos(0);
    for (auto i = 0; i < (g.qtd_vertices() / 2); ++i)
    {
        if (emp_max.mate[i] != std::numeric_limits<std::size_t>::max())
        {
            std::cout << i + 1 << "-" << emp_max.mate[i] << ((++emp_impressos == emp_max.m) ? "\n" : ", ");
        }
    }
    
    return EXIT_SUCCESS;
}
