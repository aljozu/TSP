#include<bits/stdc++.h>
#include <omp.h>
#include "reader.h"
#include <ctime>
#include <chrono>

const int INF = std::numeric_limits<int>::max();

template <typename T>
void print(std::vector<std::vector<T>> graph) {
    for(const auto& i: graph){
        for(const auto& j: i){
            if(j == INF) std::cout << "INF        ";
            else std::cout << j << "        ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename T>
struct Node
{
    std::vector<std::pair<T, T>> path;
    std::vector<std::vector<T>> matrix_reduced;
    T cost;
    int vertex;
    int level;
};

template <typename T>
Node<T>* newNode(std::vector<std::vector<T>> matrix_parent, std::vector<std::pair<T, T>> const &path, int level, int i, int j, int N)
{
    auto node = new Node<T>;
    node->path = path;
    if (level != 0)
        node->path.push_back(std::make_pair(i, j));
    node->matrix_reduced = matrix_parent;
    for (int k = 0; level != 0 && k < N; k++)
    {
        node->matrix_reduced[i][k] = INF;
        node->matrix_reduced[k][j] = INF;
    }

    node->matrix_reduced[j][0] = INF;
    node->level = level;
    node->vertex = j;
    return node;
}

template <typename T>
void reduce_row(std::vector<std::vector<T>> &matrix_reduced, std::vector<T> &row, int N)
{

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] < row[i])
                row[i] = matrix_reduced[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] != INF && row[i] != INF)
                matrix_reduced[i][j] -= row[i];
}

template <typename T>
void reduce_column(std::vector<std::vector<T>> &matrix_reduced, std::vector<T> &col, int N)
{

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] < col[j])
                col[j] = matrix_reduced[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] != INF && col[j] != INF)
                matrix_reduced[i][j] -= col[j];
}

template <typename T>
T cost_calculation(std::vector<std::vector<T>> &matrix_reduced, int N)
{
    T cost = 0;
    std::vector<T> row(N, INF);
    reduce_row(matrix_reduced, row, N);
    std::vector<T> col(N, INF);
    reduce_column(matrix_reduced, col, N);

    for (int i = 0; i < N; i++)
        cost += (row[i] != INF) ? row[i] : 0,
            cost += (col[i] != INF) ? col[i] : 0;

    return cost;
}

template <typename T>
void printPath(std::vector<std::pair<T, T>> const &list)
{
    for (int i = 0; i < list.size(); i++)
        std::cout << list[i].first + 1 << " -> " << list[i].second + 1 << std::endl;
}

template <typename T>
class comp {
public:
    bool operator()(const Node<T>* lhs, const Node<T>* rhs) const
    {
        return lhs->cost > rhs->cost;
    }
};

template <typename T>
Node<T>* TSPbranchandbound(std::vector<std::vector<T>> &adjacensyMatrix)
{
    std::priority_queue<Node<T>*, std::vector<Node<T>*>, comp<T>> pq;
    std::vector<std::pair<T, T>> v;
    int N = adjacensyMatrix[0].size();
    
    auto root = newNode<T>(adjacensyMatrix, v, 0, -1, 0, N);
    
    root->cost = cost_calculation(root->matrix_reduced, N);
    
    pq.push(root);
    
    while (!pq.empty())
    {
        auto min = pq.top();
        pq.pop();
        int i = min->vertex;
        if (min->level == N - 1)
        {
            
            min->path.push_back(std::make_pair(i, 0));
            return min;
        }

        for (int j = 0; j < N; j++)
        {
            if (min->matrix_reduced[i][j] != INF)
            {
                Node<T>* child = newNode<T>(min->matrix_reduced, min->path, min->level + 1, i, j, N);
                child->cost = min->cost + min->matrix_reduced[i][j] + cost_calculation<T>(child->matrix_reduced, N);
                pq.push(child);
            }
        }

        delete min;
    }
    return 0;
}


int main() {
    std::string nombreArchivo = "5nodos.txt";
    
    auto matrix = leerArchivo<int>(nombreArchivo);

    /*Matriz de adyacencia de prueba*/
    /*Resultado esperado: 28*/
    std::vector<std::vector<int>> ad =
    {
        { INF, 20, 30, 10, 11 },
        { 15,  INF, 16,  4,  2 },
        { 3,   5,  INF, 2,  4 },
        { 19,   6,   18,   INF, 3 },
        { 16,   4,   7,   16, INF }
    };

    std::vector<std::vector<int>> test = {
        {INF,  3,  4,  2,  7},
        {3,  INF,  4,  6,  3},
        {4,  4,  INF,  5,  8},
        {2,  6,  5,  INF,  6},
        {7,  3,  8,  6,  INF}
    };

    std::vector<std::vector<int>> matriz = {
        {INF, 633, 257,  91, 412, 150,  80, 134, 259, 505, 353, 324,  70, 211, 268, 246, 121},
        {633,   INF, 390, 661, 227, 488, 572, 530, 555, 289, 282, 638, 567, 466, 420, 745, 518},
        {257, 390,   INF, 228, 169, 112, 196, 154, 372, 262, 110, 437, 191,  74,  53, 472, 142},
        {91, 661, 228,   INF, 383, 120,  77, 105, 175, 476, 324, 240,  27, 182, 239, 237,  84},
        {412, 227, 169, 383,   INF, 267, 351, 309, 338, 196,  61, 421, 346, 243, 199, 528, 297},
        {150, 488, 112, 120, 267,   INF,  63,  34, 264, 360, 208, 329,  83, 105, 123, 364,  35},
        {80, 572, 196,  77, 351,  63,   INF,  29, 232, 444, 292, 297,  47, 150, 207, 332,  29},
        {134, 530, 154, 105, 309,  34,  29,   INF, 249, 402, 250, 314,  68, 108, 165, 349,  36},
        {259, 555, 372, 175, 338, 264, 232, 249,   INF, 495, 352,  95, 189, 326, 383, 202, 236},
        {505, 289, 262, 476, 196, 360, 444, 402, 495,   INF, 154, 578, 439, 336, 240, 685, 390},
        {353, 282, 110, 324,  61, 208, 292, 250, 352, 154,   INF, 435, 287, 184, 140, 542, 238},
        {324, 638, 437, 240, 421, 329, 297, 314,  95, 578, 435,   INF, 254, 391, 448, 157, 301},
        {70, 567, 191,  27, 346,  83,  47,  68, 189, 439, 287, 254,   INF, 145, 202, 289,  55},
        {211, 466,  74, 182, 243, 105, 150, 108, 326, 336, 184, 391, 145,   INF,  57, 426,  96},
        {268, 420,  53, 239, 199, 123, 207, 165, 383, 240, 140, 448, 202,  57,   INF, 483, 153},
        {246, 745, 472, 237, 528, 364, 332, 349, 202, 685, 542, 157, 289, 426, 483,   INF, 336},
        {121, 518, 142,  84, 297,  35,  29,  36, 236, 390, 238, 301,  55,  96, 153, 336,   INF}
    };

    std::vector<std::vector<int>> matriz2 = {
        {INF, 29, 82, 46, 68, 52, 72, 42, 51, 55, 29, 74, 23, 72, 46},
        {29, INF, 55, 46, 42, 43, 43, 23, 23, 31, 41, 51, 11, 52, 21},
        {82, 55, INF, 68, 46, 55, 23, 43, 41, 29, 79, 21, 64, 31, 51},
        {46, 46, 68, INF, 82, 15, 72, 31, 62, 42, 21, 51, 51, 43, 64},
        {68, 42, 46, 82, INF, 74, 23, 52, 21, 46, 82, 58, 46, 65, 23},
        {52, 43, 55, 15, 74, INF, 61, 23, 55, 31, 33, 37, 51, 29, 59},
        {72, 43, 23, 72, 23, 61, INF, 42, 23, 31, 77, 37, 51, 46, 33},
        {42, 23, 43, 31, 52, 23, 42, INF, 33, 15, 37, 33, 33, 31, 37},
        {51, 23, 41, 62, 21, 55, 23, 33, INF, 29, 62, 46, 29, 51, 11},
        {55, 31, 29, 42, 46, 31, 31, 15, 29, INF, 51, 21, 41, 23, 37},
        {29, 41, 79, 21, 82, 33, 77, 37, 62, 51, INF, 65, 42, 59, 61},
        {74, 51, 21, 51, 58, 37, 37, 33, 46, 21, 65, INF, 61, 11, 55},
        {23, 11, 64, 51, 46, 51, 51, 33, 29, 41, 42, 61, INF, 62, 23},
        {72, 52, 31, 43, 65, 29, 46, 31, 51, 23, 59, 11, 62, INF, 59},
        {46, 21, 51, 64, 23, 59, 33, 37, 11, 37, 61, 55, 23, 59, INF}
    };

    auto start = std::chrono::high_resolution_clock::now();	
    auto ans = TSPbranchandbound(matriz)->cost;
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
	std::cout << std::fixed << std::setprecision(5) << elapsed.count() << std::endl;
    std::cout << ans << std::endl;
}

//https://people.sc.fsu.edu/~jburkardt/datasets/tsp/tsp.html