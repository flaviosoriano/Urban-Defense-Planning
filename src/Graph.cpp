#include "Graph.hpp"

Graph::Graph() {
    this->tamanho = 0;
    this->capital = nullptr;
}

Graph::~Graph() {
    for(auto Cidade : this->cidades){
        delete Cidade.second;
    }
    for(auto batalhao : this->sccs){
        delete batalhao;
    }
}

void Graph::addEstrada(std::string Cidade1, std::string cidade2){
    //garante que a cidade está no grafo
    if (this->cidades.find(Cidade1) == this->cidades.end()){
        this->cidades[Cidade1] = new Node(Cidade1);
        this->tamanho++;
    }
    //garante que a cidade está no grafo
    if (this->cidades.find(cidade2) == this->cidades.end()){
        this->cidades[cidade2] = new Node(cidade2);
        this->tamanho++;
    }
    this->cidades[Cidade1]->estradas.push_back(new Estrada(this->cidades[Cidade1], this->cidades[cidade2]));

    this->cidades[cidade2]->InDegree++;
    this->cidades[Cidade1]->OutDegree++;
}

void Graph::addCidade(std::string Cidade){
    if (this->cidades.find(Cidade) == this->cidades.end()){
        this->cidades[Cidade] = new Node(Cidade);
        this->tamanho++;
    }
}

//Função que copia uma cidade para de um grafo para o outro
void Graph::copyCidade(Node* Cidade){
    if (this->cidades.find(Cidade->nome_Cidade) == this->cidades.end()){
        this->cidades[Cidade->nome_Cidade] = new Node(Cidade);
        this->tamanho++;
    }
}

void Graph::SetCapital(Node* capital){
    this->capital = capital;
}


std::unordered_map<std::string, Node*> Graph::getcidades() {
    return this->cidades;
}

Node* Graph::getCapital() {
    return this->capital;
}

int Graph::getTamanho() const{
    return this->tamanho;
}

void Graph::printGraph() const{
    for (auto Cidade : this->cidades){
        std::cout << Cidade.first << " -> ";
        for (auto Estrada : Cidade.second->estradas){
            std::cout << Estrada->cidade2->nome_Cidade << " ";
        }
        std::cout << std::endl;
    }
}

//Função que retorna as distancias de um grafo a partir de uma cidade
std::unordered_map<Node*,int> Graph::BFS(Node& originCidade) const{

    std::unordered_map<Node*,int> distances;
    distances.insert({&originCidade, 0});
    for(auto citie : this->cidades){
        distances.insert({citie.second, INF});
    }

    std::queue<Node*> queue;
    queue.push(&originCidade);

    while(!queue.empty()){
        Node *current = queue.front();
        queue.pop();

        for(auto Estrada : current->estradas){
            if(distances[Estrada->cidade2] == INF){
                distances[Estrada->cidade2] = distances[current] + 1;
                queue.push(Estrada->cidade2);
            }
        }
    }

    return distances;
}

//Função que retorna o caminho entre duas cidades
std::stack<Node*> Graph::BFSPath(Node& originCidade, Node& destinationCidade) const{

    //Cidade atual - cidade anterior
    std::unordered_map<Node*,Node*> path;
    path.insert({&originCidade, nullptr});
    for(auto citie : this->cidades){
        path.insert({citie.second, nullptr});
    }

    std::queue<Node*> queue;
    queue.push(&originCidade);

    while(!queue.empty()){
        Node *current = queue.front();
        queue.pop();

        if(current == &destinationCidade){
            break;
        }

        for(auto Estrada : current->estradas){
            if(path[Estrada->cidade2] == nullptr && Estrada->cidade2 != &originCidade){
                path[Estrada->cidade2] = current;
                queue.push(Estrada->cidade2);
            }
        }
    }
    //Construção do caminho
    std::stack<Node*> finalPath;
    Node* current = &destinationCidade;
    while(current != nullptr){
        finalPath.push(current);
        current = path[current];
    }
    return finalPath;
}
//Função que soma as distancias de um grafo e retorna INF caso haja uma cidade inalcançável
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
//Define a capital do grafo como a cidade que tem a menor soma das distancias para todas as outras cidades
void Graph::defineCapital(){
    int minDist = INF;
    for (auto Cidade : this->cidades){
        int dist = SomarDistancias(this->BFS(*Cidade.second));
        if (dist < minDist){
            minDist = dist;
            this->capital = Cidade.second;
        }
    }
}

//Faz uma DFS a partir da cidade indicada, marcando as cidades visitadas e adicionando a pilha
void Graph::DFS(Node* Cidade, std::unordered_map<Node*, bool>* visited, std::stack<Node*>* stack) const{
    //Marca a cidade como visitada, mas nao adiciona a pilha (semelhante a "pintar de cinza", como visto nas aulas)
    visited->at(Cidade) = true;
    //Para cada cidade vizinha, se ela nao foi visitada, chama a DFS recursivamente
    for(auto Estrada : Cidade->estradas){
        if(visited->at(Estrada->cidade2) == false){
            DFS(Estrada->cidade2, visited, stack);
        }
    }
    //Adiciona a cidade a pilha (semelhante a "pintar de preto")
    stack->push(Cidade);
}

void Graph::Kosaraju_DFS(Node* Cidade, std::unordered_map<Node*, bool>* visited, std::unordered_set<std::string>* component) const{
    visited->at(Cidade) = true;
    component->insert(Cidade->nome_Cidade);

    for(auto Estrada : Cidade->estradas){
        if(visited->at(Estrada->cidade2) == false){
            Kosaraju_DFS(Estrada->cidade2, visited, component);
        }
    }
}

//Algoritmo de Kosaraju
void Graph::Kosaraju() {

    std::unordered_map<Node*, bool> visited;
    std::stack<Node*> stack;

    for(auto Cidade : this->cidades){
        visited.insert({Cidade.second, false});
    }
    //Primeira etapa
    for(auto Cidade : this->cidades){
        if(visited.at(Cidade.second) == false){
            DFS(Cidade.second, &visited, &stack);
        }
    }
    //Segunda etapa
    Graph transposedGraph;
    for(auto Cidade : this->cidades){
        transposedGraph.addCidade(Cidade.first);
        for(auto Estrada : Cidade.second->estradas){
            transposedGraph.addEstrada(Estrada->cidade2->nome_Cidade, Cidade.first);
        }
    }
    visited.clear();
    //Terceira etapa
    for(auto Cidade : transposedGraph.cidades){
        visited.insert({Cidade.second, false});
    }
    while(!stack.empty()){
        Node* current = transposedGraph.cidades[stack.top()->nome_Cidade];
        stack.pop();
        if(!visited.at(current)){
            //percorre o grafo, adicionando as cidades ao componente(quer servirá como indice para criar o grafo desse componente isolado)
            std::unordered_set<std::string> componentIndex;
            transposedGraph.Kosaraju_DFS(current, &visited, &componentIndex);

            //cria o grafo do componente isolado
            Graph* component = new Graph();
            for(auto Cidade : componentIndex){
                component->copyCidade(this->cidades[Cidade]);
                //somente adiciona as estradas que ligam cidades do componente
                for(auto Estrada : this->cidades[Cidade]->estradas){
                    if(componentIndex.find(Estrada->cidade2->nome_Cidade) != componentIndex.end()){
                        component->addEstrada(Cidade, Estrada->cidade2->nome_Cidade);
                    }
                }
            }
            if(tamanho >= 1){
                this->sccs.push_back(component);
            }
        }
    }
}

//Função que retorna as cidades mais próximas da capital
std::vector<Node*> MaisProximo(std::unordered_map<Node*, int> distances, std::unordered_map<std::string, Node*> cidades){
    std::vector<Node*> maisProximo;
    int minDist = INF;
    for(auto distance : distances){
        //garante que a cidade é uma cidade do batalhão
        if(cidades.find(distance.first->nome_Cidade) != cidades.end()){
            //se a distancia for menor que a menor distancia encontrada até o momento, atualiza a menor distancia e reseta o vetor de cidades mais próximas
            if(distance.second < minDist){
                minDist = distance.second;
                maisProximo.clear();
                maisProximo.push_back(distance.first);
            } else if(distance.second == minDist){
                maisProximo.push_back(distance.first);
            }
        }
    }
    //retorna as cidades mais próximas da capital(caso haja mais de uma com a menor distancia encontrada)
    return maisProximo;
}

// Função que define os batalhões e suas respectivas capitais para o problema 2
void Graph::DefineBatalhoes() {

    this->Kosaraju();

    auto n_batalhoes = sccs.size();
    std::unordered_map<Node*, int> distanciasCapital = this->BFS(*this->capital);
    for(auto batalhao : sccs){
        //define o batalhão como a cidade do scc mais próxima da capital
        std::vector<Node*> capitalCandidates = MaisProximo(distanciasCapital, batalhao->cidades);
        //se houver mais de uma cidade com a mesma distancia, desempata baseado na soma das distancias para o
        if(capitalCandidates.size() > 1){
            int minDist = INF;
            for (auto Cidade : capitalCandidates){
                int dist = SomarDistancias(batalhao->BFS(*Cidade));
                if (dist < minDist){
                    minDist = dist;
                    batalhao->SetCapital(Cidade);
                }
            }
        } else {
            batalhao->SetCapital(capitalCandidates[0]);
        }
            
        //se o batalhão for a capital, não conta como batalhão
        if(batalhao->getCapital()->nome_Cidade == this->capital->nome_Cidade){
            n_batalhoes--;
        }
    }
    //impressão da resposta
    std::cout << n_batalhoes << std::endl;
    for(auto batalhao : sccs){
        if(batalhao->getCapital()->nome_Cidade != this->capital->nome_Cidade){
            std::cout << batalhao->getCapital()->nome_Cidade << std::endl;
        }
    }
}

//Conta quantos SCCs tem mais de um componente e, por tanto, são elegíveis para Patrulha
int Graph::CountSCCs(){
    int count = 0;
    for(auto batalhao : this->sccs){
        if(batalhao->tamanho > 1){
            count++;
        }
    }
    return count;
}

// Função que balanceia o grafo de um batalhão para ser possível fazer o passeio de euler
void Graph::Balancear(){
    //será utilizado depois, mas ja inicializo agora para aproveitar o loop
    //assinala aos nós um indice inteiro
    std::unordered_map<Node*, int> desbalanceadosID;
    int indice = 0;

    // Encontra quais os nos desbalanceados O(v)
    std::vector<Node*> desbalanceados;
    for(auto Cidade : this->cidades){
        if(Cidade.second->InDegree != Cidade.second->OutDegree){
            desbalanceados.push_back(Cidade.second);
            desbalanceadosID.insert({Cidade.second, indice});
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
            std::cout << std::get<0>(par)->nome_Cidade << " " << std::get<1>(par)->nome_Cidade << " " << std::get<2>(par) << std::endl;
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
            this->addEstrada(current->nome_Cidade, next->nome_Cidade);
            current = next;
        }
    }
}

//Utilizado para inverter o vetor de cidades, pois o passeio de euler é feito em ordem inversa
void Reverse(std::vector<Node*> &vector){
    std::vector<Node*> reversed;
    for(auto it = vector.rbegin(); it != vector.rend(); it++){
        reversed.push_back(*it);
    }
    vector = reversed;
}

//Algoritmo de Hieholzer
void Graph::PasseioDeEuler(Node* inicio){
    std::vector<Node*> eulerianPath;
    std::stack<Node*> stack;

    stack.push(inicio);
    while(!stack.empty()){
        Node* current = stack.top();
        std::cout << "cidade atual: " << current->nome_Cidade << std::endl;
        if(!current->estradas.empty()){
            Node* visited = current->estradas.back()->cidade2;
            current->estradas.pop_back();
            stack.push(visited);
        } else{
            std::cout << current->nome_Cidade << " adicionada" <<std::endl;
            eulerianPath.push_back(current);
            stack.pop();
        }

    }
    std::cout << std::endl;
    Reverse(eulerianPath);
    for(auto Cidade : eulerianPath){
        std::cout << Cidade->nome_Cidade << " ";
    }
}

//Função principal que chama todas as outras funções para resolver o problema 3
void Graph::Patrulhamentos(){
    std::cout << this->CountSCCs() << std::endl;
    for(auto batalhao : this->sccs){
        if(batalhao->tamanho > 1){
            std::unordered_map<Estrada*, bool> visited;
            for(auto Cidade : batalhao->cidades){
                for(auto Estrada : Cidade.second->estradas){
                    visited.insert({Estrada, false});
                }
            }
            batalhao->Balancear();
            batalhao->printGraph();
            batalhao->PasseioDeEuler(batalhao->getCapital());
            std::cout << std::endl;
        }
    }
}


