#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

int main(int argc, char** argv)
{
    if (argc != 4)
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
    std::cout << g.algoritmo_edmonds_karp(std::stoul(argv[2]), std::stoul(argv[3])) << std::endl;
    
    return EXIT_SUCCESS;
}
