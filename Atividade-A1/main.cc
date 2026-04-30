#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

void teste_ex1(const Grafo& g)
{
    std::cout << "TESTE EXERCICIO 1:\n\n";
    std::cout << "\nqtd_vertices(): " << g.qtd_vertices();
    std::cout << "\nqtd_arestas(): " << g.qtd_arestas();
    
    for (std::size_t i = 1; i <= g.qtd_vertices(); ++i)
    {
        std::cout << "\n\nVertice " << i << ":";
        std::cout << "\nGrau: " << g.grau(i);
        std::cout << "\nRotulo: " << g.rotulo(i);
        std::cout << "\nVizinhos: ";
        auto vizinhos = g.vizinhos(i);
        std::for_each(vizinhos.cbegin(), vizinhos.cend(),
                      [] (const std::size_t v) { std::cout << v << " ";});
    }
    
    std::cout << "\n\nArestas:";
    for (std::size_t i = 1; i <= g.qtd_vertices(); ++i)
    {
        for (std::size_t j = i; j <= g.qtd_vertices(); ++j)
        {
            if (g.ha_aresta(i, j))
                std::cout << "\n(" << i << ", " << j << "): " << g.peso(i, j);
        }
    }
    std::cout << "\n\n";
}

void teste_ex2(const Grafo& g)
{
    std::cout << "TESTE EXERCICIO 2:\n\n";
    for (auto i = 0; i < g.qtd_vertices(); ++i)
    {
        std::cout << "BL com origem no Vertice " << i + 1 << '\n';
        auto arvore_bl = g.arvore_busca_largura(i + 1);
        g.print_arvore_busca_largura(arvore_bl);
        std::cout << '\n';
    }
}

void teste_ex3(const Grafo& g)
{
    std::cout << "TESTE EXERCICIO 3:\n\n";
    auto ciclo = g.algoritmo_hierholzer();
    std::cout << (ciclo.ha_ciclo_euleriano ? 1 : 0) << "\n";
    for (const auto v : ciclo.ciclo_euleriano)
        std::cout << v << " ";
    std::cout << '\n';
}

int main()
{
    std::ifstream in("teste.txt");
    if (!in)
    {
        std::cerr << "erro na leitura do arquivo" << std::endl;
        return 1;
    }
    
    Grafo g = le_grafo(in);
    teste_ex1(g);
    teste_ex2(g);
    teste_ex3(g);
    
    return 0;
}
