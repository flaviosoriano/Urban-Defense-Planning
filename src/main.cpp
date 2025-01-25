#include "Graph.hpp"
#include <iostream>

using namespace std;

int main(){
    
    int nCidades = 0 , nEstradas = 0;
    cin >> nCidades; 
    cin >> nEstradas;

    Graph state = Graph();

    for (int i = 0; i < nEstradas; i++){
        string cidade1, cidade2;
        cin >> cidade1; 
        cin >> cidade2;
        state.addEstrada(cidade1, cidade2);
    }
    if(nCidades != state.getTamanho()){
        cout << "erro" << endl;
        return 0;
    }
    //resolve o problema 1
    state.defineCapital();
    cout << state.getCapital()->nome_Cidade << endl;
    //resolve o problema 2
    state.DefineBatalhoes();
    //resolve o problema 3
    state.Patrulhamentos();
    return 0;
}