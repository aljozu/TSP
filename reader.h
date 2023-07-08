#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

template <typename T>
void leerArchivo(const std::string& nombreArchivo, std::vector<std::vector<T>>& datos) {
    std::ifstream inputFile(nombreArchivo);

    if (!inputFile) {
        std::cout << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<int> fila;
        std::istringstream iss(line);

        int i, j;
        T w;
        while (iss >> i >> j >> w) {
            datos[i][j] = w;
            //datos[j][i] = w;
        }
        
    }

    inputFile.close();
}


