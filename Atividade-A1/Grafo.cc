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
        std::size_t v;
        std::string rotulo;
        is >> v;
        std::getline(is, rotulo);
        ma[i].first = rotulo.substr(1);  // retira espaco em branco
    }
    
    // le arestas
    is >> s;
    std::size_t va, vb;
    double peso;
    while (is >> va >> vb >> peso)
    {
        ma[va - 1].second[vb - 1] = peso;
        ma[vb - 1].second[va - 1] = peso;
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

std::size_t Grafo::grau(const Vertice v) const
{
    std::size_t grau{0};
    for (auto a : matriz_adjacencia[v - 1].second)
    {
        if (a != std::numeric_limits<double>::infinity())
            ++grau;
    }
    return grau;
}

std::vector<Grafo::Vertice> Grafo::vizinhos(const Vertice v) const
{
    std::vector<Vertice> vizinhos;
    for (std::size_t i = 0; i < qtd_vertices(); ++i)
    {
        if (matriz_adjacencia[v - 1].second[i] != std::numeric_limits<double>::infinity())
            vizinhos.push_back(i + 1);
    }
    return vizinhos;
}

std::map<Grafo::Vertice, std::size_t> Grafo::arvore_busca_largura(const Vertice origem) const
{
    // local struct para representar um nodo (vertice)
    struct Nodo
    {
        std::size_t nivel{std::numeric_limits<std::size_t>::max()};
        Vertice ancestral{std::numeric_limits<std::size_t>::max()};
        bool visitado{false};
    };
    
    // cada elemento de vertices representa um vertice (em ordem)
    std::vector<Nodo> vertices(qtd_vertices());
    std::queue<Vertice> q;
    
    // configurando vertice de origem
    vertices[origem - 1].nivel = 0;
    vertices[origem - 1].visitado = true;
    q.push(origem);
    
    // propagacao das visitas
    while (!q.empty())
    {
        // visitando vertice enfileirado
        Vertice atual = q.front();
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
    std::map<Vertice, std::size_t> arvore;
    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        arvore[i + 1] = vertices[i].nivel;
    }
    
    return arvore;
}

void Grafo::print_arvore_busca_largura(const std::map<Vertice, std::size_t>& arvore) const
{
    auto n = arvore.size();
    std::size_t nivel{0};
    
    while (n > 0)
    {
        std::vector<Vertice> vertices_no_nivel;
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

Grafo::RetornoHierholzer Grafo::algoritmo_hierholzer() const
{
    // sem arestas: ciclo euleriano vazio
    if (qtd_arestas() == 0)
    {
        return {true, {}};
    }
    
    // com arestas: buscar ciclo
    std::vector<std::vector<std::size_t>> C(qtd_vertices(), 
                                            std::vector<std::size_t>(qtd_vertices(), 0));
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            if (matriz_adjacencia[i].second[j] != std::numeric_limits<double>::infinity())
            {
                ++C[i][j];
            }
        }
    }
    
    Vertice v(0);
    for (auto i = 1; i <= qtd_vertices(); ++i)
    {
        if (grau(i) > 0)
        {
            v = i;
            break;
        }
    }
    
    RetornoHierholzer ciclo = buscar_subciclo(v, C);
    
    if (!ciclo.ha_ciclo_euleriano)
    {
        return {false, {}};
    }
    
    for (const auto& linha : C)
    {
        for (const auto& aresta : linha)
        {
            if (aresta != 0)
            {
                return {false, {}};
            }
        }
    }
    
    return ciclo;
}

Grafo::RetornoHierholzer Grafo::buscar_subciclo(Vertice v, std::vector<std::vector<std::size_t>>& C) const
{
    std::vector<Vertice> ciclo{v};
    
    Vertice t(v);
    do
    {
        Vertice u(0);
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            if (C[v - 1][j] > 0)  // equivalente ao else da linha 6 do algoritmo nas anotacoes
            {
                u = j + 1;
                break;
            }
        }
        
        if (u == 0)  // se u == 0, nenhuma aresta foi encontrada no for-loop acima
        {
            return {false, {}};
        }
        
        --C[v - 1][u - 1];
        --C[u - 1][v - 1];
        v = u;
        ciclo.push_back(v);
    } while (v != t);
    
    for (auto it = ciclo.begin(); it != ciclo.end(); ++it)
    {
        Vertice x = *it;
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            if (C[x - 1][j] > 0)
            {
                auto subciclo = buscar_subciclo(x, C);
                if (!subciclo.ha_ciclo_euleriano)
                {
                    return {false, {}};
                }
                ciclo.insert(it, subciclo.ciclo_euleriano.begin(), subciclo.ciclo_euleriano.end() - 1);
                break;
            }
        }
    }
    
    return {true, ciclo};
}

Grafo::RetornoFloydWarshall Grafo::algoritmo_floyd_warshall() const
{
    // matriz de custos
    std::vector<std::vector<double>> 
        matriz_custos(qtd_vertices(), std::vector<double>(qtd_vertices()));
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            matriz_custos[i][j] = (i == j) ? 0 : matriz_adjacencia[i].second[j];
        }
    }
    
    // matriz de predecessores (zero representa ausencia de predecessor (nao ha vertice 0))
    std::vector<std::vector<Vertice>> 
        matriz_predecessores(qtd_vertices(), std::vector<Vertice>(qtd_vertices()));
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            matriz_predecessores[i][j] = (matriz_adjacencia[i].second[j] == std::numeric_limits<double>::infinity()) ? 0 : i + 1;
        }
    }
    
    for (auto k = 0; k < qtd_vertices(); ++k)
    {
        for (auto i = 0; i < qtd_vertices(); ++i)
        {
            for (auto j = 0; j < qtd_vertices(); ++j)
            {
                if (matriz_custos[i][j] > matriz_custos[i][k] + matriz_custos[k][j])
                {
                    matriz_predecessores[i][j] = matriz_predecessores[k][j];
                }
                matriz_custos[i][j] = std::min(matriz_custos[i][j], matriz_custos[i][k] + matriz_custos[k][j]);
            }
        }
    }
    
    return {matriz_custos, matriz_predecessores};
}
