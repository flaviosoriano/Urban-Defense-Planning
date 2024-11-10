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
        this->cities[city1] = new Node(city1);
        this->size++;
    }
    //garante que a cidade está no grafo
    if (this->cities.find(city2) == this->cities.end()){
        this->cities[city2] = new Node(city2);
        this->size++;
    }
    this->cities[city1]->roads.push_back(new Road(this->cities[city1], this->cities[city2]));

    this->cities[city2]->InDegree++;
    this->cities[city1]->OutDegree++;
}

void Graph::addCity(std::string city){
    if (this->cities.find(city) == this->cities.end()){
        this->cities[city] = new Node(city);
        this->size++;
    }
}

void Graph::copyCity(Node* city){
    if (this->cities.find(city->City_name) == this->cities.end()){
        this->cities[city->City_name] = new Node(city);
        this->size++;
    }
}


std::unordered_map<std::string, Node*> Graph::getCities() {
    return this->cities;
}

Node* Graph::getCapital() {
    return this->capital;
}

int Graph::getSize() const{
    return this->size;
}

void Graph::printGraph() const{
    for (auto city : this->cities){
        std::cout << city.first << " -> ";
        for (auto road : city.second->roads){
            std::cout << road->city2->City_name << " ";
        }
        std::cout << std::endl;
    }
}

std::unordered_map<Node*,int> Graph::BFS(Node& originCity) const{

    std::unordered_map<Node*,int> distances;
    distances.insert({&originCity, 0});
    for(auto citie : this->cities){
        distances.insert({citie.second, INF});
    }

    std::queue<Node*> queue;
    queue.push(&originCity);

    while(!queue.empty()){
        Node *current = queue.front();
        queue.pop();

        for(auto road : current->roads){
            if(distances[road->city2] == INF){
                distances[road->city2] = distances[current] + 1;
                queue.push(road->city2);
            }
        }
    }

    return distances;
}

int SomarDistancias(std::unordered_map<Node*,int> distances){
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
    int minDist = INF;
    for (auto city : this->cities){
        int dist = SomarDistancias(this->BFS(*city.second));
        if (dist < minDist){
            minDist = dist;
            this->capital = city.second;
        }
    }
}

/*
std::vector<std::string> Graph::MaisProxCapital(){
    std::vector<std::string> cities;
    
}
*/

/*
* Faz uma DFS a partir da cidade indicada, marcando as cidades visitadas e adicionando a pilha
* @param city cidade de origem
* @param visited mapa de cidades visitadas
* @param stack pilha de cidades visitadas que será usada no algoritmo de Kosaraju 
*/
void Graph::DFS(Node* city, std::unordered_map<Node*, bool>* visited, std::stack<Node*>* stack) const{
    //Marca a cidade como visitada, mas nao adiciona a pilha (semelhante a "pintar de cinza", como visto nas aulas)
    visited->at(city) = true;
    //Para cada cidade vizinha, se ela nao foi visitada, chama a DFS recursivamente
    for(auto road : city->roads){
        if(visited->at(road->city2) == false){
            DFS(road->city2, visited, stack);
        }
    }
    //Adiciona a cidade a pilha (semelhante a "pintar de preto")
    stack->push(city);
}

void Graph::Kosaraju_DFS(Node* city, std::unordered_map<Node*, bool>* visited, std::unordered_set<std::string>* component) const{
    visited->at(city) = true;
    component->insert(city->City_name);

    for(auto road : city->roads){
        if(visited->at(road->city2) == false){
            Kosaraju_DFS(road->city2, visited, component);
        }
    }
}

void Graph::Kosaraju() {

    std::unordered_map<Node*, bool> visited;
    std::stack<Node*> stack;

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
            transposedGraph.addRoad(road->city2->City_name, city.first);
        }
    }
    visited.clear();
    //Terceira etapa
    for(auto city : transposedGraph.cities){
        visited.insert({city.second, false});
    }
    while(!stack.empty()){
        Node* current = transposedGraph.cities[stack.top()->City_name];
        stack.pop();
        if(!visited.at(current)){
            //percorre o grafo, adicionando as cidades ao componente(quer servirá como indice para criar o grafo desse componente isolado)
            std::unordered_set<std::string> componentIndex;
            transposedGraph.Kosaraju_DFS(current, &visited, &componentIndex);

            //cria o grafo do componente isolado
            Graph* component = new Graph();
            for(auto city : componentIndex){
                component->copyCity(this->cities[city]);
                //somente adiciona as estradas que ligam cidades do componente
                for(auto road : this->cities[city]->roads){
                    if(componentIndex.find(road->city2->City_name) != componentIndex.end()){
                        component->addRoad(city, road->city2->City_name);
                    }
                }
            }
            if(size >= 1){
                this->sccs.push_back(component);
            }
        }
    }
}

Node* MaisProximo(std::unordered_map<Node*, int> distances, std::unordered_map<std::string, Node*> cities){
    Node* maisProximo = nullptr;
    int minDist = INF;
    for(auto distance : distances){
        //garante que a cidade é uma cidade do batalhão
        if(distance.second < minDist && cities.find(distance.first->City_name) != cities.end()){
            minDist = distance.second;
            maisProximo = cities[distance.first->City_name];
        }
    }
    return maisProximo;
}

void Graph::DefineBatalhoes() {

    this->Kosaraju();

    auto n_batalhoes = sccs.size();
    std::unordered_map<Node*, int> distanciasCapital = this->BFS(*this->capital);
    for(auto batalhao : sccs){
        //define o batalhão como a cidade do scc mais próxima da capital
        batalhao->capital = MaisProximo(distanciasCapital, batalhao->cities);
        //se o batalhão for a capital, não conta como batalhão
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

//Conta quantos SCCs tem mais de um componente e, por tanto, são elegíveis para Patrulha
int Graph::CountSCCs(){
    int count = 0;
    for(auto batalhao : this->sccs){
        if(batalhao->size > 1){
            count++;
        }
    }
    return count;
}

void Reverse(std::vector<Node*> &vector){
    std::vector<Node*> reversed;
    for(auto it = vector.rbegin(); it != vector.rend(); it++){
        reversed.push_back(*it);
    }
    vector = reversed;
}

void Graph::Balancear(){
    std::deque<Node*> unbalancedNodesExcess;
    std::deque<Node*> unbalancedNodesLack;

    for(auto city : this->cities){
        int balance = city.second->InDegree - city.second->OutDegree;
        if(balance > 0){
            while(balance--) unbalancedNodesExcess.push_back(city.second);
        }else if(balance < 0){
            while(balance++) unbalancedNodesLack.push_back(city.second);
        }
    }

    while(!unbalancedNodesExcess.empty() && !unbalancedNodesLack.empty()){
        Node* excess = unbalancedNodesExcess.front();
        Node* lack = unbalancedNodesLack.front();
        unbalancedNodesExcess.pop_front();
        unbalancedNodesLack.pop_front();
        addRoad(excess->City_name, lack->City_name);
    }
}

void Graph::PasseioDeEuler(Node* inicio){
    std::vector<Node*> eulerianPath;
    std::stack<Node*> stack;

    stack.push(inicio);
    while(!stack.empty()){
        Node* current = stack.top();
        if(!current->roads.empty()){
            Node* visited = current->roads.back()->city2;
            current->roads.pop_back();
            stack.push(visited);
        } else{
            eulerianPath.push_back(current);
            stack.pop();
        }
    }
    
    Reverse(eulerianPath);
    for(auto city : eulerianPath){
        std::cout << city->City_name << " ";
    }
}


void Graph::Patrulhamentos(){
    std::cout << this->CountSCCs() << std::endl;
    for(auto batalhao : this->sccs){
        if(batalhao->size > 1){
            std::unordered_map<Road*, bool> visited;
            for(auto city : batalhao->cities){
                for(auto road : city.second->roads){
                    visited.insert({road, false});
                }
            }
            batalhao->printGraph();
            batalhao->Balancear();
            batalhao->printGraph();
            batalhao->PasseioDeEuler(batalhao->getCapital());
            
            std::cout << std::endl;
        }
    }
}


