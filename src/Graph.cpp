#include "Graph.hpp"

Graph::Graph(int size) : size(size){

}

Graph::~Graph(){
    this->cities.clear();
}

void Graph::addNode(std::string name){
    node *newNode = new node(name);
    cities.push_back(newNode);
}
