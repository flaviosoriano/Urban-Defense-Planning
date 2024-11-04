#include "Graph.hpp"
#include <iostream>

using namespace std;

int main(){
    int n_cities, n_roads;
    cin >> n_cities; 
    cin >> n_roads;

    Graph state = Graph();

    for (int i = 0; i < n_roads; i++){
        string city1, city2;
        cin >> city1; 
        cin >> city2;
        state.addRoad(city1, city2);
    }
    if(n_cities != state.getSize()){
        cout << "erro" << endl;
        return 0;
    }
    state.defineCapital();
    cout << "capital: " <<state.getCapital()->City_name << endl;
    
}