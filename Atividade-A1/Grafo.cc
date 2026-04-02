#include "Grafo.h"
#include <iostream>
#include <string>
#include <limits>

// input 1-based, armazenamento 0-based
Grafo le_grafo(std::istream& is = std::cin)
{
    std::string s;
    std::size_t n;
    is >> s >> n;
    
    // le vertices
    std::vector<std::pair<std::string, std::vector<double>>> 
        ma(n, std::make_pair("", std::vector<double>(n, std::numeric_limits<double>::infinity())));  // matriz de adjacencia
    for (std::size_t i = 0; i < n; ++i)
    {
        std::size_t pos;
        std::string rotulo;
        is >> pos >> rotulo;
        ma[pos - 1].first = rotulo;
    }
    
    // le arestas
    is >> s;
    std::size_t posa, posb;
    double peso;
    while (is >> posa >> posb >> peso)
    {
        ma[posa - 1].second[posb - 1] = peso;
        ma[posb - 1].second[posa - 1] = peso;
    }
    
    return Grafo(ma);
}

std::size_t Grafo::qtd_arestas() const
{
    std::size_t n{0};
    for (auto v : matriz_adjacencia)
    {
        for (auto a : v.second)
        {
            if (a != std::numeric_limits<double>::infinity()) 
                ++n;
        }
    }
    return n / 2;  // matriz simetrica (sem self loops)
}

std::size_t Grafo::grau(const Posicao pos) const
{
    std::size_t grau{0};
    for (auto a : matriz_adjacencia[pos - 1].second)
    {
        if (a != std::numeric_limits<double>::infinity())
            ++grau;
    }
    return grau;
}

std::vector<Grafo::Posicao> Grafo::vizinhos(const Posicao pos) const
{
    std::vector<Posicao> vizinhos;
    for (std::size_t i = 0; i < qtd_arestas(); ++i)
    {
        if (matriz_adjacencia[pos - 1].second[i] != std::numeric_limits<double>::infinity())
            vizinhos.push_back(i + 1);
    }
    return vizinhos;
}
