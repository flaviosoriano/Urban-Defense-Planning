#include "Graph.hpp"

Graph::Graph() {
    this->size = 0;
    this->capital = nullptr;
}

Graph::~Graph() {
    for(auto city : this->cities){
        delete city.second;
    }
    for(auto batalhao : this->sccs){
        delete batalhao;
    }
}

void Graph::addRoad(std::string city1, std::string city2){
    //garante que a cidade está no grafo
    if (this->cities.find(city1) == this->cities.end()){
        this->cities[city1] = new node(city1);
        this->size++;
    }
    //garante que a cidade está no grafo
    if (this->cities.find(city2) == this->cities.end()){
        this->cities[city2] = new node(city2);
        this->size++;
    }
    this->cities[city1]->roads.push_back(this->cities[city2]);
}

void Graph::addCity(std::string city){
    if (this->cities.find(city) == this->cities.end()){
        this->cities[city] = new node(city);
        this->size++;
    }
}

std::unordered_map<std::string, node*> Graph::getCities() {
    return this->cities;
}

node* Graph::getCapital() {
    return this->capital;
}

int Graph::getSize() const{
    return this->size;
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
        if (distance.second == INF){
            return INF;
        }
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

void Graph::BatalhaoPrincipal(node& stateCapital){
    
}

/*
* Faz uma DFS a partir da cidade indicada, marcando as cidades visitadas e adicionando a pilha
* @param city cidade de origem
* @param visited mapa de cidades visitadas
* @param stack pilha de cidades visitadas que será usada no algoritmo de Kosaraju 
*/
void Graph::DFS(node* city, std::unordered_map<node*, bool>* visited, std::stack<node*>* stack) const{
    //Marca a cidade como visitada, mas nao adiciona a pilha (semelhante a "pintar de cinza", como visto nas aulas)
    visited->at(city) = true;
    //Para cada cidade vizinha, se ela nao foi visitada, chama a DFS recursivamente
    for(auto road : city->roads){
        if(visited->at(road) == false){
            DFS(road, visited, stack);
        }
    }
    //Adiciona a cidade a pilha (semelhante a "pintar de preto")
    stack->push(city);
}

void Graph::Kosaraju_DFS(node* city, std::unordered_map<node*, bool>* visited, Graph* component) const{
    visited->at(city) = true;
    component->addCity(city->City_name);

    for(auto road : city->roads){
        component->addRoad(city->City_name, road->City_name);
        if(visited->at(road) == false){
            Kosaraju_DFS(road, visited, component);
        }
    }
}

void Graph::Kosaraju() {

    std::unordered_map<node*, bool> visited;
    std::stack<node*> stack;

    for(auto city : this->cities){
        visited.insert({city.second, false});
    }
    //Primeira etapa
    for(auto city : this->cities){
        if(visited.at(city.second) == false){
            DFS(city.second, &visited, &stack);
        }
    }
    //Segunda etapa
    Graph transposedGraph;
    for(auto city : this->cities){
        transposedGraph.addCity(city.first);
        for(auto road : city.second->roads){
            transposedGraph.addRoad(road->City_name, city.first);
        }
    }
    visited.clear();
    //Terceira etapa
    for(auto city : transposedGraph.cities){
        visited.insert({city.second, false});
    }
    while(!stack.empty()){
        node* current = transposedGraph.cities[stack.top()->City_name];
        stack.pop();
        if(!visited.at(current)){
            Graph* component = new Graph();
            transposedGraph.Kosaraju_DFS(current, &visited, component);
            int size = component->getSize();
            if(size >= 1){
                this->sccs.push_back(component);
            }
        }
    }
}

void Graph::DefineBatalhoes() {

    this->Kosaraju();

    auto n_batalhoes = sccs.size();
    for(auto batalhao : sccs){
        batalhao->defineCapital();
        if(batalhao->getCapital()->City_name == this->capital->City_name){
            n_batalhoes--;
        }
    }


    //impressão da resposta
    std::cout << n_batalhoes << std::endl;
    for(auto batalhao : sccs){
        if(batalhao->getCapital()->City_name != this->capital->City_name){
            std::cout << batalhao->getCapital()->City_name << std::endl;
        }
    }
}


