#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include "Grafo.h"

int main(int argc, char** argv)
{
    if (argc != 3)
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
    
    // gera arvore de busca em largura
    Grafo::Vertice v(static_cast<std::size_t>(std::stoull(argv[2])));
    if (v > g.qtd_vertices())
    {
        std::cerr << "Grafo nao contem vertice " << argv[2] << "." << std::endl;
        return EXIT_FAILURE;
    }
    auto arvore = g.arvore_busca_largura(v);
    
    // imprime resultado
    auto n = arvore.size();
    std::size_t nivel{0};
    
    while (n > 0)
    {
        std::vector<Grafo::Vertice> vertices_no_nivel;
        bool arvore_processada{true};
        for (auto v : arvore)
        {
            if (v.second == nivel) 
            {
                vertices_no_nivel.push_back(v.first);
                --n;
                arvore_processada = false;
            }
        }
        
        // caso os demais vertices pertencam a outra componente conexa
        if (arvore_processada && n > 0)
        {
            break;
        }
        
        std::sort(vertices_no_nivel.begin(), vertices_no_nivel.end());
        std::cout << nivel << ": ";
        for (std::size_t i = 0; i < vertices_no_nivel.size(); ++i)
        {
            std::cout << vertices_no_nivel[i] << (i != vertices_no_nivel.size() - 1 ? "," : "");
        }
        std::cout << '\n';
        
        ++nivel;
    }
    
    return EXIT_SUCCESS;
}
