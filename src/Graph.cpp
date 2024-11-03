#include "Graph.hpp"

Graph::Graph(int size) : size(size){

}

Graph::~Graph(){
    for (auto city : this->cities){
        for(auto road : city.second->roads){
            delete road;
        }
        city.second->roads.clear();
        delete city.second;
    }
    cities.clear();
}

void Graph::addRoad(std::string city1, std::string city2){
    //garante que a cidade está no grafo
    if (this->cities.find(city1) == this->cities.end()){
        this->cities[city1] = new node(city1);
    }
    //garante que a cidade está no grafo
    if (this->cities.find(city2) == this->cities.end()){
        this->cities[city2] = new node(city2);
    }
    this->cities[city1]->roads.push_back(this->cities[city2]);
}

std::unordered_map<std::string, node*> Graph::getCities() {
    return this->cities;
}

node* Graph::getCapital() {
    return this->capital;
}

void Graph::printGraph() const{
    for (auto city : this->cities){
        std::cout << city.first << " -> ";
        for (auto road : city.second->roads){
            std::cout << road->City_name << " ";
        }
        std::cout << std::endl;
    }
}

double Graph::BFS(node& originCity) const{

    std::unordered_map<node*,double> distances;
    distances.insert({&originCity, 0});
    for(auto citie : this->cities){
        distances.insert({citie.second, INF});
    }

    std::queue<node*> queue;
    queue.push(&originCity);

    while(!queue.empty()){
        node *current = queue.front();
        queue.pop();

        for(auto road : current->roads){
            if(distances[road] == INF){
                distances[road] = distances[current] + 1;
                queue.push(road);
            }
        }
    }

    double totalDistance = 0;
    for(auto distance : distances){
        totalDistance += distance.second;
    }

    return totalDistance;
}


void Graph::defineCapital(){
    double minDist = INF;
    for (auto city : this->cities){
        double dist = this->BFS(*city.second);
        if (dist < minDist){
            minDist = dist;
            this->capital = city.second;
        }
    }
}
