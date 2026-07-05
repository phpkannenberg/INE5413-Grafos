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
    
    // le arestas/arcos
    is >> s;
    std::size_t va, vb;
    double peso;
    while (is >> va >> vb >> peso)
    {
        ma[va - 1].second[vb - 1] = peso;
        if (s == "*edges")
        {
            ma[vb - 1].second[va - 1] = peso;
        }
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
        std::size_t nivel;
        Vertice ancestral;
        bool visitado;
    };
    
    // cada elemento de vertices representa um vertice (em ordem)
    std::vector<Nodo> vertices(qtd_vertices(), {std::numeric_limits<std::size_t>::max(),
                                                std::numeric_limits<std::size_t>::max(),
                                                false});
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
            if (!vertices[vizinho - 1].visitado)
            {
                // conhecendo vertices ainda nao visitados
                vertices[vizinho - 1].nivel = vertices[atual - 1].nivel + 1;
                vertices[vizinho - 1].ancestral = atual;
                vertices[vizinho - 1].visitado = true;
                q.push(vizinho);
            }
        }
    }
    
    // gera arvore de busca (map)
    std::map<Vertice, std::size_t> arvore;
    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        arvore[i + 1] = vertices[i].nivel;
    }
    
    return arvore;
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
    
    for (auto i = 0; i < ciclo.size(); ++i)
    {
        Vertice x = ciclo[i];
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            if (C[x - 1][j] > 0)
            {
                auto subciclo = buscar_subciclo(x, C);
                if (!subciclo.ha_ciclo_euleriano)
                {
                    return {false, {}};
                }
                ciclo.erase(ciclo.begin() + i);  // evita duplicatas
                ciclo.insert(ciclo.begin() + i, subciclo.ciclo_euleriano.begin(), subciclo.ciclo_euleriano.end() - 1);
                i += subciclo.ciclo_euleriano.size() - 1;  // evita processamentos desnecessarios
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

std::vector<std::pair<Grafo::Vertice, Grafo::Vertice>> Grafo::algoritmo_kosaraju_sharir() const
{
    // chama DFS para computar os tempos de termino para cada vertice
    std::vector<VerticeCFC> vertices(DFS());
    
    // grafo transposto
    Grafo grafo_transposto(transpor_grafo());
    
    // DFS adaptado
    DFS_adaptado(grafo_transposto, vertices);
    
    std::sort(vertices.begin(), vertices.end(), [] (const VerticeCFC& a, const VerticeCFC& b) { return a.vertice < b.vertice; });
    std::vector<std::pair<Vertice, Vertice>> ancestrais;
    for (const auto& v : vertices)
    {
        ancestrais.push_back(std::make_pair(v.vertice, v.ancestral));
    }
    
    return ancestrais;
}

std::vector<Grafo::VerticeCFC> Grafo::DFS() const
{
    // configurando todos os vertices
    std::vector<VerticeCFC> vertices(qtd_vertices());
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        vertices[i].vertice = i + 1;
        vertices[i].conhecido = false;
        vertices[i].tempo_inicio = std::numeric_limits<std::size_t>::max();
        vertices[i].tempo_termino = std::numeric_limits<std::size_t>::max();
        vertices[i].ancestral = std::numeric_limits<std::size_t>::max();
    }
    
    // configurando tempo de inicio
    std::size_t tempo(0);
    
    for (auto& v : vertices)
    {
        if (!v.conhecido)
        {
            DFS_visit(v.vertice, vertices, tempo);
        }
    }
    
    return vertices;
}

void Grafo::DFS_adaptado(const Grafo& grafo_transposto, std::vector<VerticeCFC>& vertices) const
{
    // ordena vertices em ordem decrescente de F (tempo_termino)
    std::sort(vertices.begin(), vertices.end(), [&] (const VerticeCFC a, const VerticeCFC b) { return a.tempo_termino > b.tempo_termino; });
    
    // configurando todos os vertices
    for (auto& v: vertices)
    {
        v.conhecido = false;
        v.tempo_inicio = std::numeric_limits<std::size_t>::max();
        v.tempo_termino = std::numeric_limits<std::size_t>::max();
        v.ancestral = std::numeric_limits<std::size_t>::max();
    }
    
    std::size_t tempo(0);
    
    for (auto& v : vertices)
    {
        if (!v.conhecido)
        {
            grafo_transposto.DFS_visit(v.vertice, vertices, tempo);
        }
    }
}

void Grafo::DFS_visit(Vertice id, std::vector<VerticeCFC>& vertices, std::size_t& tempo) const
{
    // seleciona VerticeCFC com base no id
    VerticeCFC& u_cfc = *std::find_if(vertices.begin(), vertices.end(), 
                                     [id] (const VerticeCFC& a) { return a.vertice == id; });
    
    u_cfc.conhecido = true;
    ++tempo;
    u_cfc.tempo_inicio = tempo;
    
    // encontra vizinhos de u ainda nao conhecidos e executa DFS-visit
    for (auto& v : vizinhos(u_cfc.vertice))
    {
        for (auto& w_cfc : vertices)
        {
            if (w_cfc.vertice == v && !w_cfc.conhecido)
            {
                w_cfc.ancestral = u_cfc.vertice;
                DFS_visit(w_cfc.vertice, vertices, tempo);
            }
        }
    }
    
    ++tempo;
    u_cfc.tempo_termino = tempo;
}

Grafo Grafo::transpor_grafo() const
{
    MatrizAdjacencia ma(qtd_vertices(), std::make_pair("", std::vector<double>(qtd_vertices(), std::numeric_limits<double>::infinity())));
    
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        ma[i].first = matriz_adjacencia[i].first;
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            if (matriz_adjacencia[i].second[j] != std::numeric_limits<double>::infinity())
            {
                ma[j].second[i] = matriz_adjacencia[i].second[j];
            }
        }
    }
    
    return Grafo(ma);
}

const std::vector<Grafo::Vertice> Grafo::DFS_ordenacao_topologica() const
{
    std::vector<VerticeOT> vertices(qtd_vertices());
    
    // configurando todos os vertices
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        vertices[i].vertice = i + 1;
        vertices[i].conhecido = false;
        vertices[i].tempo_inicio = std::numeric_limits<std::size_t>::max();
        vertices[i].tempo_termino = std::numeric_limits<std::size_t>::max();
    }
    
    // configurando tempo de inicio
    std::size_t tempo(0);
    
    // criando lista com os vertices ordenados topologicamente (estrutura O)
    std::vector<Vertice> ordenacao;
    
    for (auto& v : vertices)
    {
        if (!v.conhecido) DFS_visit_ot(v.vertice, vertices, tempo, ordenacao);
    }
    
    return ordenacao;
}

void Grafo::DFS_visit_ot(const Vertice& id_origem, std::vector<VerticeOT>& vertices, std::size_t& tempo, std::vector<Vertice>& ordenacao) const
{
    // seleciona VerticeOT com base no id_origem
    VerticeOT& v_ot = *std::find_if(vertices.begin(), vertices.end(), 
                                   [id_origem] (const VerticeOT& a) { return a.vertice == id_origem; });
                                   
    v_ot.conhecido = true;
    ++tempo;
    v_ot.tempo_inicio = tempo;
    
    for (const auto& u : vizinhos(v_ot.vertice))
    {
        for (auto& u_ot : vertices)
        {
            if (u == u_ot.vertice && !u_ot.conhecido)
            {
                DFS_visit_ot(u_ot.vertice, vertices, tempo, ordenacao);
            }
        }
    }
    
    ++tempo;
    v_ot.tempo_termino = tempo;
    ordenacao.insert(ordenacao.begin(), v_ot.vertice);
}

std::vector<std::pair<Grafo::Vertice, Grafo::Vertice>> Grafo::algoritmo_prim() const
{
    struct VerticePrim
    {
        Vertice vertice;
        Vertice ancestral;
        std::size_t chave;
    };

    std::vector<VerticePrim> vertices(qtd_vertices());
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        vertices[i].vertice = i + 1;
        vertices[i].ancestral = std::numeric_limits<std::size_t>::max();
        vertices[i].chave = std::numeric_limits<std::size_t>::max();
    }
    
    auto vertices_controle(vertices);
    if (!vertices.empty()) vertices_controle[0].chave = 0;
    
    while (!vertices_controle.empty())
    {
        std::sort(vertices_controle.begin(), vertices_controle.end(), 
            [] (const VerticePrim& a, const VerticePrim& b) { return a.chave < b.chave; });
        
        VerticePrim u(*vertices_controle.begin());  // copia    
        vertices_controle.erase(vertices_controle.begin());
        
        for (const auto v : vizinhos(u.vertice))
        {
            auto v_it = std::find_if(vertices_controle.begin(), vertices_controle.end(), [v] (const VerticePrim& a) { return a.vertice == v; });
            if (v_it != vertices_controle.end() && peso(u.vertice, (*v_it).vertice) < (*v_it).chave)
            {
                auto v_original_it(std::find_if(vertices.begin(), vertices.end(), [v] (const VerticePrim& a) { return a.vertice == v; })); 
                (*v_original_it).ancestral = u.vertice;
                (*v_original_it).chave = peso(u.vertice, (*v_it).vertice);
                (*v_it).chave = peso(u.vertice, (*v_it).vertice);
            }
        }
    }
    
    std::vector<std::pair<Vertice, Vertice>> ancestrais;
    for (const auto& v : vertices)
        ancestrais.push_back(std::make_pair(v.vertice, v.ancestral));
        
    return ancestrais;
}

double Grafo::algoritmo_edmonds_karp(const Vertice s, const Vertice t) const
{
    // gera novo grafo para representar capacidades na rede residual
    auto rede_res(rede_residual());
    
    double F(0);
    
    while (true)
    {
        auto p(BFS_edmonds_karp(s, t, rede_res));
        if (p.empty()) break;  // sai do loop caso nao haja mais caminhos aumentantes
        
        // identificando a capacidade do caminho
        double fluxo(std::numeric_limits<double>::infinity());
        for (auto i = 0; i < p.size() - 1; ++i)
        {
            if (rede_res.peso(p[i], p[i + 1]) < fluxo)
            {
                fluxo = rede_res.peso(p[i], p[i + 1]);
            }
        }
        
        F += fluxo;
        
        // atualizando a capacidade residual
        for (auto i = 0; i < p.size() - 1; ++i)
        {
            auto nova_cap(matriz_adjacencia[p[i] - 1].second[p[i + 1] - 1] - fluxo);
            rede_res.set_peso(p[i], p[i + 1], nova_cap);
        }
    }
    
    return F;
}

std::vector<Grafo::Vertice> Grafo::BFS_edmonds_karp(const Vertice s, const Vertice t, Grafo& rede_residual) const
{
    struct VerticeEK
    {
        bool conhecido;
        Vertice ancestral;
    };
    
    // configurando vertices
    std::vector<VerticeEK> vertices(qtd_vertices());
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        vertices[i].conhecido = false;
        vertices[i].ancestral = std::numeric_limits<std::size_t>::max();
    }
    vertices[s - 1].conhecido = true;
    
    std::queue<Vertice> q;
    q.push(s);
    
    while (!q.empty())
    {
        auto u(q.front());
        q.pop();
        
        for (auto v : vizinhos(u))
        {
            if (!vertices[v - 1].conhecido && rede_residual.peso(u, v) > 0)
            {
                vertices[v - 1].conhecido = true;
                vertices[v - 1].ancestral = u;
                
                // sorvedouro encontrado
                if (v == t)
                {
                    std::vector<Vertice> p({t});
                    auto w(t);
      
                    while (w != s)
                    {
                        w = vertices[w - 1].ancestral;
                        p.insert(p.begin(), w);
                    }
                    
                    return p;
                }
                
                q.push(v);
            }
        }
    }
    
    return std::vector<Vertice>();  // retorna vetor vazio caso nao encontre caminho aumentante
}

Grafo Grafo::rede_residual() const
{
    // matriz para rede residual
    MatrizAdjacencia ma(qtd_vertices(), std::make_pair("", std::vector<double>(qtd_vertices(), std::numeric_limits<double>::infinity())));
    
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        ma[i].first = rotulo(i + 1);  // define rotulo
        for (auto j = 0; j < qtd_vertices(); ++j)
        {
            ma[i].second[j] = (peso(i + 1, j + 1) == std::numeric_limits<double>::infinity() ? 0 : peso(i + 1, j + 1));  // define capacidade
            /*
             * fluxo nao sera definido no sentido j -> i pois sera calculado a partir da diferenca 
             * entre peso da matriz do grafo original e o peso da matriz do grafo da rede residual
             */
        }
    }
    
    return Grafo(ma);
}

Grafo::RetornoHopcroftKarp Grafo::algoritmo_hopcroft_karp() const
{    
    auto null(qtd_vertices() + 1);
    
    std::vector<VerticeHK> vertices
        (qtd_vertices() + 1, {std::numeric_limits<std::size_t>::max(), null});
    std::size_t m(0);
    
    while (BFS_hopcroft_karp(vertices))
    {
        for (auto i = 0; i < (qtd_vertices() / 2); ++i)
        {
            if (vertices[i].mate == null)
            {
                if (DFS_hopcroft_karp(vertices, i + 1))
                {
                    ++m;
                }
            }
        }
    }
    
    std::vector<Vertice> mate(qtd_vertices());
    for (auto i = 0; i < qtd_vertices(); ++i)
    {
        mate[i] = vertices[i].mate;
    }
    
    return {m, mate};
}

bool Grafo::BFS_hopcroft_karp(std::vector<VerticeHK>& vertices) const
{
    auto null(qtd_vertices() + 1);
    
    std::queue<Vertice> q;
    
    for (auto i = 0; i < (qtd_vertices() / 2); ++i)
    {
        if (vertices[i].mate == null)
        {
            vertices[i].distancia = 0;
            q.push(i + 1);
        }
        else
        {
            vertices[i].distancia = std::numeric_limits<std::size_t>::max();
        }
    }
    
    vertices[null - 1].distancia = std::numeric_limits<std::size_t>::max();
    
    while (!q.empty())
    {
        auto x(q.front());
        q.pop();
        
        if (vertices[x - 1].distancia < vertices[null - 1].distancia)
        {
            for (auto y : vizinhos(x))
            {
                auto mate_y(vertices[y - 1].mate);
                if (vertices[mate_y - 1].distancia == std::numeric_limits<std::size_t>::max())
                {
                    vertices[mate_y - 1].distancia = vertices[x - 1].distancia + 1;
                    q.push(mate_y);
                }
            }
        }
    }
    
    return vertices[null - 1].distancia != std::numeric_limits<std::size_t>::max();
}

bool Grafo::DFS_hopcroft_karp(std::vector<VerticeHK>& vertices, const Vertice x) const
{
    auto null(qtd_vertices() + 1);
    
    if (x != null)
    {
        for (auto y : vizinhos(x))
        {
            auto mate_y(vertices[y - 1].mate);
            if (vertices[mate_y - 1].distancia == (vertices[x - 1].distancia + 1))
            {
                if (DFS_hopcroft_karp(vertices, mate_y))
                {
                    vertices[y - 1].mate = x;
                    vertices[x - 1].mate = y;
                    return true;
                }
            }
        }
        vertices[x - 1].distancia = std::numeric_limits<std::size_t>::max();
        return false;
    }
    
    return true;
}
