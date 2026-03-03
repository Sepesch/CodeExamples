#include <iostream>
#include <cmath>

using namespace std;

// Функция f(x) = e^x - arccos(sqrt(x))
double f(double x) {
    if (x < 0.0 || x > 1.0) {
        cerr << "x должен быть в диапазоне [0, 1]" << endl;
        exit(1);
    }
    return exp(x) - acos(sqrt(x));
}

// Метод хорд
double chord_method(double a, double b, double eps, int &iter) {
    int max_iter = 1000;
    if (f(a) * f(b) >= 0) {
        cerr << "Функция должна иметь разные знаки на концах интервала [a, b]" << endl;
        exit(1);
    }

    double x = a; // Начальное приближение

    while (abs(f(x)) > eps && iter < max_iter) {
        x = (a * f(b) - b * f(a)) / (f(b) - f(a)); // Формула метода хорд
        if (f(x) * f(a) < 0) {
            b = x;
        } else {
            a = x;
        }
        iter++;
    }

    if (iter == max_iter) {
        cout << "Достигнуто максимальное количество итераций." << endl;
    }

    return x;
}

int main() {
    // Параметры
    double a = 0.0; // Левая граница интервала
    double b = 1.0; // Правая граница интервала
    int iter;
    for(int i = 1; i<=30; i++){
        double eps = pow(0.1, i); // Точность
    // Вычисление корня
        double root = chord_method(a, b, eps, iter);
        cout.precision(i);
        cout<<i<<"\t\t"<<iter<< '\n';
        iter=0;
    }

    return 0;
}