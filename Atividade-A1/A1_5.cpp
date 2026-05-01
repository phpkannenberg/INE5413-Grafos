#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

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
    
    // busca caminhos minimos (execucao algoritmo de floyd-warshall)
    auto fw = g.algoritmo_floyd_warshall();
    
    // imprime resultado
    for (auto i = 0; i < g.qtd_vertices(); ++i)
    {
        std::cout << i + 1 << ":";
        for (auto j = 0; j < g.qtd_vertices(); ++j)
        {
            std::cout << fw.matriz_pesos[i][j]
                      << (j != g.qtd_vertices() - 1 ? "," : "\n"); 
        }
    }

    return EXIT_SUCCESS;
}
