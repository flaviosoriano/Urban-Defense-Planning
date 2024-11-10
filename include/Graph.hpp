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

#define INF std::numeric_limits<int>::max()

struct Road;

struct Node {
    std::string City_name;
    std::vector<Road*> roads;
    int InDegree;
    int OutDegree;

    Node(std::string name){
        City_name = name;
        roads = std::vector<Road*>();
        InDegree = 0;
        OutDegree = 0;
    }
    Node(Node *city){
        City_name = city->City_name;
        roads = std::vector<Road*>();
        InDegree = 0;
        OutDegree = 0;
    }
};

struct Road {
    Node *city1;
    Node *city2;

    Road(Node *city1, Node *city2){
        this->city1 = city1;
        this->city2 = city2;
    }
};

class Graph {
private:

    std::unordered_map<std::string, Node*> cities;
    int size;
    std::vector<Graph*> sccs;

    Node *capital;

public:
    Graph();
    ~Graph();

    void addRoad(std::string city1, std::string city2);
    void addCity(std::string city);
    void copyCity(Node* city);

    std::unordered_map<std::string, Node*> getCities();
    Node* getCapital();
    int getSize() const;
    void printGraph() const;

    std::unordered_map<Node*,int> BFS(Node& originCity) const;
    void defineCapital();

    void DFS(Node* city, std::unordered_map<Node*, bool>* visited, std::stack<Node*>* stack) const;
    void Kosaraju_DFS(Node* city, std::unordered_map<Node*, bool>* visited, std::unordered_set<std::string>* component) const;
    void Kosaraju();
    void DefineBatalhoes() ;

    int CountSCCs();
    void Balancear();
    void PasseioDeEuler(Node* inicio);
    void Patrulhamentos();
};

#endif