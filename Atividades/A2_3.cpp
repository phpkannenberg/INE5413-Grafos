#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

void imprimir_agm(const std::vector<std::pair<Grafo::Vertice, Grafo::Vertice>>& agm, const Grafo& g)
{
    struct Aresta
    {
        Grafo::Vertice u;
        Grafo::Vertice v;
    };
    
    double custo(0);
    std::vector<Aresta> arestas;
    
    for (auto i = 0; i < agm.size(); ++i)
    {
        if (agm[i].second == std::numeric_limits<std::size_t>::max()) continue;
        custo += g.peso(agm[i].first, agm[i].second);
        arestas.push_back({agm[i].first, agm[i].second});
    }
    
    std::cout << custo << "\n";
    for (auto i = 0; i < arestas.size(); ++i)
    {
        std::cout << arestas[i].u << "-" << arestas[i].v << (i == arestas.size() - 1 ? "\n" : ", ");
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
    auto agm(g.algoritmo_prim());  // vetor com arvore geradora minima (organizado por ancestrais)
    imprimir_agm(agm, g);
    
    return EXIT_SUCCESS;
}
