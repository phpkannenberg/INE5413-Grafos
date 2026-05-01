#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Grafo.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Quantidade incorreta de argumentos." << std::endl;
        return EXIT_FAILURE;
    }
    
    std::ifstream is(argv[1]);
    if (!is)
    {
        std::cerr << "Erro ao abrir arquivo " << argv[1] << "." << std::endl;
        return EXIT_FAILURE;
    }
    
    // le grafo
    auto g = le_grafo(is);
    
    // busca ciclo euleriano (execucao do algoritmo de hierholzer)
    auto hierholzer = g.algoritmo_hierholzer();
    
    // imprime resultado da busca
    if (!hierholzer.ha_ciclo_euleriano)
    {
        std::cout << "0\n";
    }
    else
    {
        std::cout << "1\n";
        for (auto i = 0; i < hierholzer.ciclo_euleriano.size(); ++i)
        {
            if (i == hierholzer.ciclo_euleriano.size() - 1)
            {
                std::cout << hierholzer.ciclo_euleriano[i] << '\n';
                continue;
            }
            std::cout << hierholzer.ciclo_euleriano[i] << ",";
        }
    }

    return EXIT_SUCCESS;
}
