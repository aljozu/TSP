#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

const int INF = std::numeric_limits<int>::max();

template <typename T>
std::vector<std::vector<T>> leerArchivo(const std::string& nombreArchivo) {
    std::vector<std::vector<T>> matriz;
    std::ifstream archivo(nombreArchivo);

    if (!archivo) {
        std::cout << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return matriz;
    }

    std::string linea;
    while (getline(archivo, linea)) {
        std::vector<T> fila;
        std::istringstream iss(linea);
        T valor;

        while (iss >> valor) {
            if(valor == 0)
                fila.push_back(INF);
            else
                fila.push_back(valor);
        }

        matriz.push_back(fila);
    }

    archivo.close();
    return matriz;
}

template <typename T>
void writeToCSV(const std::string& filename, std::pair<T, double>& data) {
    std::ofstream file("output.csv", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Write data
    file << data.first << "," << data.second << std::endl;
    

    file.close();
    std::cout << "Data written to " << filename << std::endl;
}