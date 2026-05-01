#ifndef A1_GRAFO_H
#define A1_GRAFO_H

#include <vector>
#include <string>
#include <istream>
#include <limits>
#include <map>

class Grafo
{
public:
    typedef std::size_t Vertice;
    typedef std::vector<std::pair<std::string, std::vector<double>>> MatrizAdjacencia;
    
    // EXERCICIO 1
    // leitura do grafo a partir de arquivo eh feita por funcao le_grafo (declarada fora do escopo da classe)
    Grafo(): matriz_adjacencia() { }  // default constructor
    Grafo(const MatrizAdjacencia& ma): matriz_adjacencia(ma) { }
    std::size_t qtd_vertices() const 
        { return matriz_adjacencia.size(); }
    std::size_t qtd_arestas() const;
    std::size_t grau(const Vertice v) const;
    const std::string& rotulo(const Vertice v) const 
        { return matriz_adjacencia[v - 1].first; }
    std::vector<Vertice> vizinhos(const Vertice v) const;
    bool ha_aresta(const Vertice va, const Vertice vb) const
        { return matriz_adjacencia[va - 1].second[vb - 1] != std::numeric_limits<double>::infinity(); }
    double peso(const Vertice va, const Vertice vb) const
        { return matriz_adjacencia[va - 1].second[vb - 1]; }
    
    // EXERCICIO 2
    // retorna um map representando arvore de busca 
    // pairs do map representam vertice e nivel da busca em que vertice foi visitado
    std::map<Vertice, std::size_t> arvore_busca_largura(const Vertice origem) const;
    
    // EXERCICIO 3
    // struct para encapsular estruturas retornadas pelo algoritmo de Hierholzer
    struct RetornoHierholzer
    {
        bool ha_ciclo_euleriano;
        std::vector<Vertice> ciclo_euleriano;
    };
    RetornoHierholzer algoritmo_hierholzer() const;
    
    // EXERCICIO 5
    // struct para encapsular estruturas retornadas pelo algoritmo de Floyd-Warshall
    struct RetornoFloydWarshall
    {
        std::vector<std::vector<double>> matriz_pesos;
        std::vector<std::vector<Vertice>> matriz_predecessores;
    };
    RetornoFloydWarshall algoritmo_floyd_warshall() const;
    
private:
    // cada elemento representa um vertice
    // pair.first retorna o rotulo
    // pair.second retorna vetor com peso da transicao para cada vertice
    const MatrizAdjacencia matriz_adjacencia;
    
    // funcao auxiliar chamada por Grafo::algoritmo_hierholzer
    RetornoHierholzer buscar_subciclo(Vertice v, std::vector<std::vector<std::size_t>>& C) const;
};

Grafo le_grafo(std::istream& is);

#endif
