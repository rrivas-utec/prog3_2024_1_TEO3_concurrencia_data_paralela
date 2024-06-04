#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>

void summarizar(std::vector<int>& source, int& result, int start, int stop) {
    result = std::accumulate(std::next(source.begin(), start), std::next(source.begin(), stop), 0);
}

int main() {
    int n = 10045;
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 1);
    auto total_esperado = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "total esperado: " << total_esperado << std::endl;

    // Definir el rango
    auto nt = std::thread::hardware_concurrency();
    auto range = n / nt;
    auto residuo = n % nt;
    std::cout << "rango: " << range << " residuo: " << residuo << std::endl;

    std::vector<int> subtotales(nt);
    std::vector<std::jthread> vt;
    vt.reserve(nt);
    for (int i = 0; i < nt; ++i) {
        vt.emplace_back(summarizar, std::ref(v), std::ref(subtotales[i]), i*range, (i+1)*range);
    }
    for (int i = 0; i < nt; ++i) {
        vt[i].join();
    }

    auto total_obtenido = std::accumulate(subtotales.begin(), subtotales.end(), 0);
    total_obtenido += std::accumulate(next(v.begin(), nt*range), v.end(), 0);
    std::cout << "total obtenido: " << total_obtenido << std::endl;


    return 0;
}
