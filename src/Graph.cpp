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

int Graph::BFS(node& originCity) const{

    std::unordered_map<node*,int> distances;
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

    int totalDistance = 0;
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

void Graph::DFS(node& originCity, std::map<node*, int>* distances) const{
    
    distances->insert({&originCity, 0});
    for(auto citie : this->cities){
        distances->insert({citie.second, INF});
    }

    std::stack<std::pair<node*, bool>> stack;
    stack.push({&originCity, false});

    while(!stack.empty()){
        std::pair<node*, bool> current = stack.top();
        stack.pop();
        if(!current.second){
            current.second = true;
            for(auto road : current.first->roads){
                distances->at(road) = distances->at(current.first) + 1;
                stack.push({road, false});
            }
        }
    }
}

void Graph::Kusaraju_DFS(std::multimap<int, node*>* distances, std::multimap<int, node*>* scc) const{
    
    // nó, visitado, conectado com o ultimo no da pilha
    std::stack<std::tuple<node*, bool, bool>> stack;

    for(auto distance : *distances){
        stack.push({distance.second, false, false});
    }

    int nComponent = -1;
    while(!stack.empty()){
        // nó, visitado, conectado com o ultimo no da pilha
        std::tuple<node*, bool , bool> current = stack.top();
        stack.pop();
 
        if(!std::get<1>(current)){

            std::get<1>(current) = true;
            for(auto road : std::get<0>(current)->roads){
                stack.push({road, false, true});
            }
            // caso o nó não esteja conectado com o ultimo nó da pilha
            // ele é um novo componente
            if(!std::get<2>(current)){
                nComponent++;
            }
            scc->insert({nComponent, std::get<0>(current)});
        }
    }
}

void Graph::Kusaraju(std::multimap<int, node*>* scc) const{

    std::map<node*, int> distances;
    DFS(*this->capital, &distances);


    Graph transposedGraph = Graph(this->size);
    for(auto city : this->cities){
        for(auto road : city.second->roads){
            transposedGraph.addRoad(road->City_name, city.first);
        }
    }
    // Cria um multimap para armazenar ordenadamente as distâncias que servirão 
    // como lista para o proximo DFS
    std::multimap<int, node*> ordenedDistances;
    for(auto distance : distances){
        ordenedDistances.insert({distance.second, distance.first});
    }
    transposedGraph.Kusaraju_DFS(&ordenedDistances, scc);
}
    


