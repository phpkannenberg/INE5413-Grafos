#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

void imprimir_ot(const std::vector<Grafo::Vertice>& ot, const Grafo& g)
{
    for (auto i = 0; i < ot.size(); ++i)
    {
        std::cout << g.rotulo(ot[i]) << (i == ot.size() - 1 ? "\n" : " , ");
    }
}

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
    
    auto g(le_grafo(is));  // le grafo
    auto ot(g.DFS_ordenacao_topologica());  // obtem vetor com ordenacao topologica
    imprimir_ot(ot, g);
    
    return EXIT_SUCCESS;
}
