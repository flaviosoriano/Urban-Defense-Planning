#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <limits>

#include <string>

#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <map>
#include <unordered_set>
#include <set>

#define INF std::numeric_limits<int>::max()

struct Estrada;

struct Node {
    std::string nome_Cidade;
    std::vector<Estrada*> estradas;
    int InDegree;
    int OutDegree;

    Node(std::string nome){
        nome_Cidade = nome;
        estradas = std::vector<Estrada*>();
        InDegree = 0;
        OutDegree = 0;
    }
    Node(Node *cidade){
        nome_Cidade = cidade->nome_Cidade;
        estradas = std::vector<Estrada*>();
        InDegree = 0;
        OutDegree = 0;
    }
};

struct Estrada {
    Node *cidade1;
    Node *cidade2;

    Estrada(Node *cidade1, Node *cidade2){
        this->cidade1 = cidade1;
        this->cidade2 = cidade2;
    }
};

class Graph {
private:

    std::unordered_map<std::string, Node*> cidades;
    int tamanho;
    std::vector<Graph*> sccs;

    Node *capital;

public:
    Graph();
    ~Graph();

    void addEstrada(std::string cidade1, std::string cidade2);
    void addCidade(std::string cidade);
    void copyCidade(Node* cidade);
    void SetCapital(Node* capital);

    std::unordered_map<std::string, Node*> getcidades();
    Node* getCapital();
    int getTamanho() const;
    void printGraph() const;

    std::unordered_map<Node*,int> BFS(Node& cidadeInicio) const;
    void defineCapital();

    void DFS(Node* cidade, std::unordered_map<Node*, bool>* visitados, std::stack<Node*>* stack) const;
    void Kosaraju_DFS(Node* cidade, std::unordered_map<Node*, bool>* visitados, std::unordered_set<std::string>* component) const;
    void Kosaraju();
    void DesempateBatalhao(std::vector<Node*> &capitalCandidates);
    void DefineBatalhoes();

    int CountSCCs();
    std::stack<Node*> BFSPath(Node& cidadeInicio, Node& cidadeFinal) const;
    void Balancear();
    void PasseioDeEuler(Node* inicio);
    void Patrulhamentos();
};

#endif