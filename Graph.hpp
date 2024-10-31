#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>

struct node{
    std::string City_name;
    node *next;
    node *road;

    node(std::string name){
        City_name = name;
        next = nullptr;
        road = nullptr;
    }
};

class Graph
{
private:

    node *head;
    node *tail;
    int size;

public:
    Graph(int size);
    ~Graph();
};

#endif
