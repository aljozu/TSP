#include<bits/stdc++.h>
#include <omp.h>
#include "reader.h"
#include <ctime>
#include <chrono>
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

void printPath(std::vector<double> graph) {
    for(const auto& i: graph){
        std::cout << i << " ";
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
    for (int i = 0; i < array.size(); i++) {
        array[i] -= number;
    }
}

void substract(std::vector<std::vector<double>>& graph, int pos, std::vector<double>& low){
    for (int i = 0; i < graph[0].size(); i++) {
        graph[i][pos] -= low[pos];
    }
}

void subtractNumberColumn(std::vector<std::vector<double>>& graph, std::vector<double>& low) {
    for (int i = 0; i < low.size(); i++) {
        if(low[i] != 0){
            substract(graph, i, low);
        }
    }
}

double setInfDist(std::vector<std::vector<double>>& graph, int node1, int node2){
    auto cost = graph[node1][node2];
    graph[node2][node1] = INF;
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
    costo += std::accumulate(low.begin(), low.end(), 0.0, [](double accumulator, double value) {
        return accumulator + (value != INF ? value : 0);
    });

    subtractNumberColumn(graph, low);
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
            //printPath(min->path);
            return min->cost;
        }


        #pragma omp parallel for shared(min)
        for(int j = 0; j < N; j++){
            if(min->costMatrix[i][j] != INF){
                auto child = new city(min->costMatrix, min->path, j, 0, min->level+1);
                child->path.push_back(j);
                auto cnodes = setInfDist(child->costMatrix, i, j);
                child->cost = min->cost + normalizar(child->costMatrix) + cnodes;
                #pragma omp critical
                pq.push(child);
            }
        }
        delete min;
    }
    return 0;  
}


int main() {
    int n = 10;  // numero de ciudades

    std::string nombreArchivo = "xqf131.tsp";
    omp_set_num_threads(4);
    int N = 81;
    std::vector<std::vector<double>>matrix(N,std::vector<double>(N,INF));
    
    leerArchivo(nombreArchivo, matrix);

    /*Matriz de adyacencia de prueba*/
    /*Resultado esperado: 28*/
    std::vector<std::vector<double>> ad =
    {
        { INF, 20, 30, 10, 11 },
        { 15,  INF, 16,  4,  2 },
        { 3,   5,  INF, 2,  4 },
        { 19,   6,   18,   INF, 3 },
        { 16,   4,   7,   16, INF }
    };

    auto start = std::chrono::high_resolution_clock::now();	
    auto ans = TSPbranchandbound(matrix);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
	std::cout << std::fixed << std::setprecision(5) << elapsed.count() << std::endl;
    std::cout << ans << std::endl;
}
