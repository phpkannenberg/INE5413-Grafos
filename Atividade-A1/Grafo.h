#ifndef A1_GRAFO_H
#define A1_GRAFO_H

#include <vector>
#include <string>
#include <istream>
#include <limits>

class Grafo
{
public:
    typedef std::size_t Posicao;
    typedef std::vector<std::pair<std::string, std::vector<double>>> MatrizAdjacencia;
    
    Grafo(): matriz_adjacencia() { }  // default constructor
    Grafo(const Grafo& g) = default;  // copy constructor
    Grafo(const MatrizAdjacencia& ma): matriz_adjacencia(ma) { }
    ~Grafo() = default;  // destructor
    
    std::size_t qtd_vertices() const 
        { return matriz_adjacencia.size(); }
    std::size_t qtd_arestas() const;
    std::size_t grau(const Posicao pos) const;
    const std::string& rotulo(const Posicao pos) const 
        { return matriz_adjacencia[pos - 1].first; }
    std::vector<Posicao> vizinhos(const Posicao pos) const;
    bool ha_aresta(const Posicao posa, const Posicao posb) const
        { return matriz_adjacencia[posa - 1].second[posb - 1] != std::numeric_limits<double>::infinity(); }
    double peso(const Posicao posa, const Posicao posb) const
        { return matriz_adjacencia[posa - 1].second[posb - 1]; }
private:
    // cada elemento representa um vertice
    // pair.first retorna o rotulo
    // pair.second retorna vetor com peso da transicao para cada vertice
    const MatrizAdjacencia matriz_adjacencia;
};

Grafo le_grafo(std::istream& is);  

#endif
