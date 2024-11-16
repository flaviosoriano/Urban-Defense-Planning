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

void Graph::SetCapital(Node* capital){
    this->capital = capital;
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

std::stack<Node*> Graph::BFSPath(Node& originCity, Node& destinationCity) const{

    //Cidade atual - cidade anterior
    std::unordered_map<Node*,Node*> path;
    path.insert({&originCity, nullptr});
    for(auto citie : this->cities){
        path.insert({citie.second, nullptr});
    }

    std::queue<Node*> queue;
    queue.push(&originCity);

    while(!queue.empty()){
        Node *current = queue.front();
        queue.pop();

        if(current == &destinationCity){
            break;
        }

        for(auto road : current->roads){
            if(path[road->city2] == nullptr && road->city2 != &originCity){
                path[road->city2] = current;
                queue.push(road->city2);
            }
        }
    }
    //Construção do caminho
    std::stack<Node*> finalPath;
    Node* current = &destinationCity;
    while(current != nullptr){
        finalPath.push(current);
        current = path[current];
    }
    return finalPath;
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

std::vector<Node*> MaisProximo(std::unordered_map<Node*, int> distances, std::unordered_map<std::string, Node*> cities){
    std::vector<Node*> maisProximo;
    int minDist = INF;
    for(auto distance : distances){
        //garante que a cidade é uma cidade do batalhão
        if(distance.second <= minDist && cities.find(distance.first->City_name) != cities.end()){
            minDist = distance.second;
            maisProximo.push_back(cities[distance.first->City_name]);
        }
    }
    //retorna as cidades mais próximas da capital(caso haja mais de uma com a menor distancia encontrada)
    return maisProximo;
}


void Graph::DefineBatalhoes() {

    this->Kosaraju();

    auto n_batalhoes = sccs.size();
    std::unordered_map<Node*, int> distanciasCapital = this->BFS(*this->capital);
    for(auto batalhao : sccs){
        //define o batalhão como a cidade do scc mais próxima da capital
        std::vector<Node*> capitalCandidates = MaisProximo(distanciasCapital, batalhao->cities);
        //se houver mais de uma cidade com a mesma distancia, desempata baseado na soma das distancias para o
        if(capitalCandidates.size() > 1){
            int minDist = INF;
            for (auto city : capitalCandidates){
                int dist = SomarDistancias(batalhao->BFS(*city));
                if (dist < minDist){
                    minDist = dist;
                    this->capital = city;
                }
            }
        } else{
            batalhao->SetCapital(capitalCandidates[0]);
        }
            
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


void Graph::Balancear(){
    //será utilizado depois, mas ja inicializo agora para aproveitar o loop
    //assinala aos nós um indice inteiro
    std::unordered_map<Node*, int> desbalanceadosID;
    int indice = 0;

    // Encontra quais os nos desbalanceados O(v)
    std::vector<Node*> desbalanceados;
    for(auto city : this->cities){
        if(city.second->InDegree != city.second->OutDegree){
            desbalanceados.push_back(city.second);
            desbalanceadosID.insert({city.second, indice});
            indice++;
        }
    }

    // Define todos os pares possíveis de nós desbalanceados O(V²)
    std::vector<std::tuple<Node*, Node*, int>> paresDesbalanceados; //guarda também a distancia entre eles
    for(int i = 0; i < (int)desbalanceados.size(); i++){
        std::unordered_map<Node*, int> distances = this->BFS(*desbalanceados[i]);
        int firstDegree = desbalanceados[i]->InDegree - desbalanceados[i]->OutDegree;
        //So faz o par se é necessária uma aresta do primeiro para o segundo
        for(int j = 0; j < (int)desbalanceados.size(); j++){
            int secondDegree = desbalanceados[j]->InDegree - desbalanceados[j]->OutDegree;
            if(i != j && (firstDegree > 0 && secondDegree < 0)){
            paresDesbalanceados.push_back({desbalanceados[i], desbalanceados[j], distances[desbalanceados[j]]});
            }
        }
    }

    // Cria conjuntos de pares que contemplam todos os nós desbalanceados O(V²)
    std::set<std::vector<std::tuple<Node*, Node*, int>>> conjuntos;
    std::vector<int> added (desbalanceados.size(), 0);
    for(auto parAtual : paresDesbalanceados){
        std::vector<std::tuple<Node*, Node*, int>> conjuntoAtual;
        added.assign(desbalanceados.size(), 0);
        conjuntoAtual.push_back(parAtual);
        added[desbalanceadosID[std::get<0>(parAtual)]] = 1;
        added[desbalanceadosID[std::get<1>(parAtual)]] = 1;

        for(auto parIterado : paresDesbalanceados){
            int firstID = desbalanceadosID[std::get<0>(parIterado)];
            int secondID = desbalanceadosID[std::get<1>(parIterado)];

            if(!added[firstID] && !added[secondID]){
                conjuntoAtual.push_back(parIterado);
                added[firstID] = 1;
                added[secondID] = 1;
            }
            if(conjuntoAtual.size() == desbalanceados.size()/2){
                break;
            }
        }
        
        conjuntos.insert(conjuntoAtual);
        
    }

    //imprime os conjuntos
    for(auto conjunto : conjuntos){
        for(auto par : conjunto){
            std::cout << std::get<0>(par)->City_name << " " << std::get<1>(par)->City_name << " " << std::get<2>(par) << std::endl;
        }
        std::cout << std::endl;
    }

    // Escolhe o conjunto com menor distancia total O(V²)
    std::vector<std::tuple<Node*, Node*, int>> menorConjunto;
    int menorDistancia = INF;
    for(auto conjunto : conjuntos){
        int distanciaAtual = 0;
        for(auto par : conjunto){
            distanciaAtual += std::get<2>(par);
        }
        if(distanciaAtual < menorDistancia){
            menorDistancia = distanciaAtual;
            menorConjunto = conjunto;
        }
    }

    // Adiciona as arestas ao grafo O(V)
    for(auto par : menorConjunto){
        std::stack path = this->BFSPath(*std::get<0>(par), *std::get<1>(par));
        Node* current = path.top();
        path.pop();
        while(!path.empty()){
            Node* next = path.top();
            path.pop();
            this->addRoad(current->City_name, next->City_name);
            current = next;
        }
    }
}

void Reverse(std::vector<Node*> &vector){
    std::vector<Node*> reversed;
    for(auto it = vector.rbegin(); it != vector.rend(); it++){
        reversed.push_back(*it);
    }
    vector = reversed;
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
            batalhao->Balancear();
            batalhao->PasseioDeEuler(batalhao->getCapital());
            std::cout << std::endl;
        }
    }
}


