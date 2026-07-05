#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

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
    auto coloracao(g.algoritmo_lawler());
    
    std::cout << coloracao.numero_cromatico << '\n';
    for (auto i = 0; i < g.qtd_vertices(); ++i)
    {
        std::cout << coloracao.coloracao[i] << ((i == g.qtd_vertices() - 1) ? "\n" : ", ");
    }
    
    return EXIT_SUCCESS;
}
