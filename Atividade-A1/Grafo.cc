#include "Grafo.h"
#include <iostream>
#include <string>
#include <limits>
#include <map>
#include <set>
#include <queue>
#include <cstdlib>
#include <algorithm>

// input 1-based, armazenamento 0-based
Grafo le_grafo(std::istream& is = std::cin)
{
    std::string s;
    std::size_t n;
    is >> s >> n;
    std::getline(is, s);
    
    // le vertices
    std::vector<std::pair<std::string, std::vector<double>>> 
        ma(n, std::make_pair("", std::vector<double>(n, std::numeric_limits<double>::infinity())));  // matriz de adjacencia
    for (std::size_t i = 0; i < n; ++i)
    {
        std::size_t pos;
        std::string rotulo;
        is >> pos;
        std::getline(is, rotulo);
        ma[i].first = rotulo.substr(1);  // retira espaco em branco
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
    for (std::size_t i = 0; i < qtd_vertices(); ++i)
    {
        if (matriz_adjacencia[pos - 1].second[i] != std::numeric_limits<double>::infinity())
            vizinhos.push_back(i + 1);
    }
    return vizinhos;
}

std::map<Grafo::Posicao, std::size_t> Grafo::arvore_busca_largura(const Posicao origem) const
{
    // local struct para representar um vertice
    struct Vertice
    {
        std::size_t nivel{std::numeric_limits<std::size_t>::max()};
        Posicao ancestral{std::numeric_limits<std::size_t>::max()};
        bool visitado{false};
    };
    
    // cada elemento de vertices representa um vertice (em ordem)
    std::vector<Vertice> vertices(qtd_vertices());
    std::queue<Posicao> q;
    
    // configurando vertice de origem
    vertices[origem - 1].nivel = 0;
    vertices[origem - 1].visitado = true;
    q.push(origem);
    
    // propagacao das visitas
    while (!q.empty())
    {
        // visitando vertice enfileirado
        Posicao atual = q.front();
        q.pop();
        for (auto vizinho : vizinhos(atual))
        {
            if (vertices[vizinho - 1].visitado == false)
            {
                // conhecendo vertices ainda nao visitados
                vertices[vizinho - 1].nivel = vertices[atual - 1].nivel + 1;
                vertices[vizinho - 1].ancestral = atual;
                vertices[vizinho - 1].visitado = true;
                q.push(vizinho);
            }
        }
    }
    
    // gera map
    std::map<Posicao, std::size_t> arvore;
    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        arvore[i + 1] = vertices[i].nivel;
    }
    
    return arvore;
}

void print_arvore_busca_largura(const std::map<Grafo::Posicao, std::size_t>& arvore)
{
    auto n = arvore.size();
    std::size_t nivel{0};
    
    while (n > 0)
    {
        std::vector<Grafo::Posicao> vertices_no_nivel;
        for (auto v : arvore)
        {
            if (v.second == nivel) 
            {
                vertices_no_nivel.push_back(v.first);
                --n;
            }
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
}
