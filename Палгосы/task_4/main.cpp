#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
void testMatrixMultiplication();
// void testSorting();
int main() {
    std::cout << "ЛР4: Алгоритмы GPGPU\n" << std::endl;
    
    // Задание 4.1
    testMatrixMultiplication();
    
    // Задание 4.2
    // testSorting();
    
    return 0;
}