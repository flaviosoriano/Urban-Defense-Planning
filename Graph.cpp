#include "Graph.hpp"

Graph::Graph(int size) : size(size)
{
    head = nullptr;
    tail = nullptr;
}

Graph::~Graph()
{
    node *temp = head;
    while (temp != nullptr)
    {
        node *next = temp->next;
        delete temp;
        temp = next;
    }
}
