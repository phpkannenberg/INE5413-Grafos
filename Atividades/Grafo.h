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
    
    // A1-EXERCICIO 1
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
    
    // A1-EXERCICIO 2
    // retorna um map representando arvore de busca 
    // pairs do map representam vertice e nivel da busca em que vertice foi visitado
    std::map<Vertice, std::size_t> arvore_busca_largura(const Vertice origem) const;
    
    // A1-EXERCICIO 3
    // struct para encapsular estruturas retornadas pelo algoritmo de Hierholzer
    struct RetornoHierholzer
    {
        bool ha_ciclo_euleriano;
        std::vector<Vertice> ciclo_euleriano;
    };
    RetornoHierholzer algoritmo_hierholzer() const;
    
    // A1-EXERCICIO 5
    // struct para encapsular estruturas retornadas pelo algoritmo de Floyd-Warshall
    struct RetornoFloydWarshall
    {
        std::vector<std::vector<double>> matriz_pesos;
        std::vector<std::vector<Vertice>> matriz_predecessores;
    };
    RetornoFloydWarshall algoritmo_floyd_warshall() const;
    
    // A2-EXERCICIO 1
    // struct para encapsular informacoes importantes para cada vertice no algoritmo de Kosaraju-Sharir
    struct VerticeCFC
    {
        Vertice vertice;
        bool conhecido;
        std::size_t tempo_inicio;
        std::size_t tempo_termino;
        Vertice ancestral;
    };
    std::vector<std::pair<Vertice, Vertice>> algoritmo_kosaraju_sharir() const;  // retorna vetor de ancestrais
    
    // A2-EXERCICIO 2
    // struct para encapsular informacoes importantes para cada vertice na ordenacao topologica
    struct VerticeOT
    {
        Vertice vertice;
        bool conhecido;
        std::size_t tempo_inicio;
        std::size_t tempo_termino;
    };
    const std::vector<Vertice> DFS_ordenacao_topologica() const;
    
    // A2-EXERCICIO 3
    std::vector<std::pair<Vertice, Vertice>> algoritmo_prim() const;
    
    // A3-EXERCICIO 1
    double algoritmo_edmonds_karp(const Vertice s, const Vertice t) const;
    
private:
    // cada elemento representa um vertice
    // pair.first retorna o rotulo
    // pair.second retorna vetor com peso da transicao para cada vertice
    MatrizAdjacencia matriz_adjacencia;
    void set_peso(const Vertice va, const Vertice vb, const double novo_peso)
        { matriz_adjacencia[va - 1].second[vb - 1] = novo_peso; }
    
    // funcao auxiliar chamada por Grafo::algoritmo_hierholzer
    RetornoHierholzer buscar_subciclo(Vertice v, std::vector<std::vector<std::size_t>>& C) const;
    
    // funcoes auxiliares para busca de componentes fortemente conectadas
    std::vector<VerticeCFC> DFS() const;
    void DFS_adaptado(const Grafo& grafo_transposto, std::vector<VerticeCFC>& vertices) const;
    void DFS_visit(Vertice u, std::vector<VerticeCFC>& vertices, std::size_t& tempo) const;
    Grafo transpor_grafo() const;
    
    // funcao auxiliar chamada por Grafo::DFS_ordenacao_topologica
    void DFS_visit_ot(const Vertice& id_origem, std::vector<VerticeOT>& vertices, std::size_t& tempo, std::vector<Vertice>& ordenacao) const;
    
    // funcoes auxiliar chamada por Grafo::algoritmo_edmonds_karp
    std::vector<Vertice> BFS_edmonds_karp(const Vertice s, const Vertice t, Grafo& rede_residual) const;
    Grafo rede_residual() const;
};

Grafo le_grafo(std::istream& is);

#endif
