#include "Graph.hpp"
#include <iostream>

using namespace std;

int main(){

    //apenas para depuração
    //freopen("inputs/testCase05.txt", "r", stdin);

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
    state.defineCapital();
    cout << state.getCapital()->nome_Cidade << endl;
    state.DefineBatalhoes();
    state.Patrulhamentos();
    return 0;
}