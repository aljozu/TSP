#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <omp.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <numeric>
#include <omp.h>
#include <queue>

const double INF = std::numeric_limits<double>::max();

void print(std::vector<std::vector<double>> graph) {
    for(const auto& i: graph){
        for(const auto& j: i){
            if(j == INF) std::cout << "INF        ";
            else std::cout << j << "        ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<double> findLowestElementColumn(std::vector<std::vector<double>>& graph) {
    double lowest = INF;
    std::vector<double> low(graph.size(), INF);
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph.size(); ++j) {
            if(graph[i][j] < low[j]){
                low[j] = graph[i][j];
            }
        }
    }
    return low;
}

void subtractNumberRow(std::vector<double>& array, double number) {
    #pragma omp parallel for
    for (int i = 0; i < array.size(); i++) {
        array[i] -= number;
    }
}

void substract(std::vector<std::vector<double>>& graph, int pos, std::vector<double>& low){
    for (int i = 0; i < graph[0].size(); i++) {
        #pragma omp critical
        graph[i][pos] -= low[pos];
    }
}

void subtractNumberColumn(std::vector<std::vector<double>>& graph, std::vector<double>& low) {
    #pragma omp for
    for (int i = 0; i < low.size(); i++) {
        if(low[i] != 0){
            substract(graph, i, low);
        }
    }
}

double setInfDist(std::vector<std::vector<double>>& graph, int node1, int node2){
    auto cost = graph[node1][node2];
    graph[node2][node1] = INF;
    #pragma omp for
    for(int i = 0; i < graph[0].size(); i++){
        graph[node1][i] = INF;
        graph[i][node2] = INF;
    }
    return cost;
}

double normalizar(std::vector<std::vector<double>>& graph){
    double costo = 0.0;

    //Normalizar las filas
    for(auto &i: graph){
        if(std::find(i.begin(), i.end(), 0) == i.end()){
            auto lowestElement = std::min_element(i.begin(), i.end());
            if(*lowestElement != INF)
                costo += *lowestElement;
            subtractNumberRow(i, *lowestElement);
        }
    }

    
    //Normalizar columnas
    auto low = findLowestElementColumn(graph);
    /* costo += std::accumulate(low.begin(), low.end(), 0.0, [](double accumulator, double value) {
            return accumulator + (value != INF ? value : 0);
        }); */
    for(auto x: low){
        if(x != INF)
            costo += x;
    }
    subtractNumberColumn(graph, low);
    //std::cout << "aaaaaaaa: " << costo << std::endl;
    return costo;
}   

struct city{
    std::vector<std::vector<double>> costMatrix;
    std::vector<double> path;
    int vertex;
    double cost;
    int level;

    city (std::vector<std::vector<double>> costMatrix, std::vector<double> path, int vertex, double cost, int level) :
    costMatrix{costMatrix}, path{path}, vertex{vertex}, cost{cost}, level{level} {};

};

class comp {
public:
    bool operator()(const city* lhs, const city* rhs) const
    {
        return lhs->cost > rhs->cost;
    }
};

std::vector<double> prr;
double TSPbranchandbound(std::vector<std::vector<double>> graph) {
    std::priority_queue<city*, std::vector<city*>, comp> pq;
    std::vector<double> cities;
    city *root = new city(graph, cities, 0, 0, 0);
    root->cost=normalizar(root->costMatrix);
    root->path.push_back(0);
    pq.push(root);  
    auto N = graph[0].size();
    while(!pq.empty()){
        auto min = pq.top();
        pq.pop();
        auto i = min->vertex;
        if(min->level == N - 1){
            min->path.push_back(i);
            prr = min->path;
            return min->cost;
        }

        #pragma omp for
        for(int j = 0; j < N; j++){
            if(min->costMatrix[i][j] != INF){
                auto child = new city(min->costMatrix, min->path, j, 0, min->level+1);
                child->path.push_back(j);
                auto cnodes = setInfDist(child->costMatrix, i, j);
                //print(child->costMatrix);
                //std::cout << normalizar(child->costMatrix) << std::endl;
                child->cost = min->cost + normalizar(child->costMatrix) + cnodes;
                //std::cout << "sdaaaaa" << child->cost << std::endl;
                pq.push(child);
            }
        }
        delete min;
    }
    return 0;  
}


int main() {
    int n = 10;  // numero de ciudades

    std::vector<std::vector<double>> graph = {  
        {INF,4.6,9.8,11.5,3.7,13.4,19.7,6.2,7.5,8.7},
        {4.6,INF,4.3,7.7,7.7,17.5,13.8,2.9,5.8,5.2},
        {9.8,4.3,INF,3.2,14.1,28.6,16.2,8.3,7.5,6.4},
        {11.5,7.7,3.2,INF,15.2,27.4,17.8,9.4,10.2,9.1},
        {3.7,7.7,14.1,15.2,INF,12.1,18.6,9.5,10.8,17.9},
        {13.4,17.5,28.6,27.4,12.1,INF,28.7,18.2,19.9,27.7},
        {19.7,13.8,16.2,17.8,18.6,28.7,INF,12.2,17.3,10.9},
        {6.2,2.9,8.3,9.4,9.5,18.2,12.2,INF,7.2,4.5},
        {7.5,5.8,7.5,10.2,10.8,19.9,17.3,7.2,INF,9.5},
        {8.7,5.2,6.4,9.1,17.9,27.7,10.9,4.5,9.5,INF}
    };

   std::vector<std::vector<double>> ad =
    {
        { INF, 20, 30, 10, 11 },
        { 15,  INF, 16,  4,  2 },
        { 3,   5,  INF, 2,  4 },
        { 19,   6,   18,   INF, 3 },
        { 16,   4,   7,   16, INF }
    };

    auto ans = TSPbranchandbound(ad);
    std::cout << ans << std::endl;
}
