#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Функция для создания скрипта GNUplot и построения графиков
void plotMatrixMultiplicationData() {
    // Данные из таблицы
    std::vector<int> threads = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    
    // Время выполнения для разных размеров матриц (в мс)
    std::vector<double> size100 = {22.51, 12.04, 9.40, 7.55, 7.09, 6.61, 6.18, 6.40, 6.76, 6.58, 6.27, 6.16, 6.54, 6.69, 6.42, 7.94};
    std::vector<double> size200 = {132.38, 84.05, 62.92, 53.05, 47.77, 45.68, 42.33, 40.52, 41.06, 42.06, 40.97, 39.90, 39.37, 39.67, 38.54, 39.41};
    std::vector<double> size300 = {449.71, 275.62, 202.80, 158.74, 150.70, 136.13, 125.27, 114.93, 125.99, 123.58, 119.95, 118.64, 121.29, 122.04, 121.96, 120.06};
    std::vector<double> size400 = {1102.14, 664.81, 471.37, 369.97, 348.73, 323.36, 295.01, 280.73, 290.74, 279.93, 277.10, 273.71, 286.99, 284.91, 284.12, 284.08};
    std::vector<double> size500 = {2149.32, 1293.83, 920.50, 711.14, 681.47, 632.33, 586.93, 534.99, 563.26, 571.79, 581.19, 547.42, 580.94, 558.35, 575.56, 528.42};
    
    // Создаем файлы с данными
    std::ofstream dataFile("matrix_data.dat");
    
    if (dataFile.is_open()) {
        dataFile << "# Threads\t100x100\t200x200\t300x300\t400x400\t500x500\n";
        for (size_t i = 0; i < threads.size(); ++i) {
            dataFile << threads[i] << "\t" 
                     << size100[i] << "\t" 
                     << size200[i] << "\t" 
                     << size300[i] << "\t" 
                     << size400[i] << "\t" 
                     << size500[i] << "\n";
        }
        dataFile.close();
    }
    
    // Создаем скрипт для GNUplot
    std::ofstream plotScript("plot_matrix.gnu");
    
    if (plotScript.is_open()) {
        plotScript << "set terminal pngcairo size 1200,800 enhanced font 'Arial,12'\n";
        plotScript << "set output 'matrix_multiplication_performance.png'\n\n";
        
        plotScript << "set title 'Производительность многопоточного умножения матриц' font 'Arial,16'\n";
        plotScript << "set xlabel 'Количество потоков' font 'Arial,14'\n";
        plotScript << "set ylabel 'Время выполнения (мс)' font 'Arial,14'\n\n";
        
        plotScript << "set key top right outside\n";
        plotScript << "set grid xtics ytics\n";
        plotScript << "set xtics 1\n";
        plotScript << "set logscale y\n\n";  // Логарифмическая шкала для лучшего отображения
        
        plotScript << "plot 'matrix_data.dat' using 1:2 with linespoints title '100×100' linewidth 2 pointtype 1, \\\n";
        plotScript << "     '' using 1:3 with linespoints title '200×200' linewidth 2 pointtype 2, \\\n";
        plotScript << "     '' using 1:4 with linespoints title '300×300' linewidth 2 pointtype 3, \\\n";
        plotScript << "     '' using 1:5 with linespoints title '400×400' linewidth 2 pointtype 4, \\\n";
        plotScript << "     '' using 1:6 with linespoints title '500×500' linewidth 2 pointtype 5\n\n";
        
        plotScript << "set terminal qt size 1200,800\n";
        plotScript << "set title 'Производительность многопоточного умножения матриц (линейная шкала)'\n";
        plotScript << "unset logscale y\n";
        plotScript << "replot\n";
        
        plotScript.close();
    }
    
    // Запускаем GNUplot
    system("gnuplot -persist plot_matrix.gnu");
}

// Альтернативная версия с построением отдельных графиков для каждого размера матрицы
void plotSeparateGraphs() {
    std::ofstream separateScript("plot_separate.gnu");
    
    if (separateScript.is_open()) {
        separateScript << "set terminal pngcairo size 1000,600 enhanced font 'Arial,10'\n";
        separateScript << "set output 'matrix_comparison.png'\n\n";
        
        separateScript << "set multiplot layout 2,3 title 'Сравнение производительности для разных размеров матриц' font 'Arial,14'\n\n";
        
        separateScript << "set xlabel 'Потоки'\n";
        separateScript << "set ylabel 'Время (мс)'\n";
        separateScript << "set grid\n";
        separateScript << "set xtics 2\n\n";
        
        // График для 100x100
        separateScript << "set title '100×100'\n";
        separateScript << "plot 'matrix_data.dat' using 1:2 with linespoints title '' linewidth 2\n\n";
        
        // График для 200x200
        separateScript << "set title '200×200'\n";
        separateScript << "plot 'matrix_data.dat' using 1:3 with linespoints title '' linewidth 2\n\n";
        
        // График для 300x300
        separateScript << "set title '300×300'\n";
        separateScript << "plot 'matrix_data.dat' using 1:4 with linespoints title '' linewidth 2\n\n";
        
        // График для 400x400
        separateScript << "set title '400×400'\n";
        separateScript << "plot 'matrix_data.dat' using 1:5 with linespoints title '' linewidth 2\n\n";
        
        // График для 500x500
        separateScript << "set title '500×500'\n";
        separateScript << "plot 'matrix_data.dat' using 1:6 with linespoints title '' linewidth 2\n\n";
        
        separateScript << "unset multiplot\n";
        separateScript.close();
        
        system("gnuplot -persist plot_separate.gnu");
    }
}

int main() {
    std::cout << "Генерация графиков производительности умножения матриц...\n";
    
    // Создаем основной график
    plotMatrixMultiplicationData();
    
    // Создаем отдельные графики для сравнения
    std::cout << "Графики сохранены в файлы:\n";
    std::cout << "- matrix_comparison.png\n";
    std::cout << "Исходные данные в matrix_data.dat\n";
    
    return 0;
}