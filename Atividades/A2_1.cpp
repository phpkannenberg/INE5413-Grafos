#include "Grafo.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

Grafo::Vertice encontrar_raiz(const Grafo::Vertice& v, const std::map<Grafo::Vertice, Grafo::Vertice>& pais)
{
    if (pais.at(v) == std::numeric_limits<std::size_t>::max()) return v;
    return encontrar_raiz(pais.at(v), pais);
}

std::map<Grafo::Vertice, std::vector<Grafo::Vertice>> obter_cfcs(const std::vector<std::pair<Grafo::Vertice, Grafo::Vertice>>& ancestrais)
{
    // mapeamento de cada vertice com seu ancestral (melhor performance para enderecar de cada vertice)
    std::map<Grafo::Vertice, Grafo::Vertice> pais;
    for (const auto& pair : ancestrais)
    {
        pais[pair.first] = pair.second;
    }
    
    // encontra raiz de cada vertice na arvore e adiciona ao vetor correspondente aquela raiz
    std::map<Grafo::Vertice, std::vector<Grafo::Vertice>> cfcs;
    for (const auto& pair : ancestrais)
    {
        auto raiz(encontrar_raiz(pair.first, pais));
        cfcs[raiz].push_back(pair.first);
    }
    
    return cfcs;
}

void imprimir_cfcs(const std::map<Grafo::Vertice, std::vector<Grafo::Vertice>>& cfcs)
{
    for (const auto& cfc : cfcs)
    {
        for (auto i = 0; i < cfc.second.size(); ++i)
        {
            std::cout << cfc.second[i] << (i == cfc.second.size() - 1 ? "" : ",");
        }
        std::cout << "\n";
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
    auto floresta_ancestrais(g.algoritmo_kosaraju_sharir());  // obtem floresta de CFCs
    auto cfcs(obter_cfcs(floresta_ancestrais));
    imprimir_cfcs(cfcs);
    
    return EXIT_SUCCESS;
}
