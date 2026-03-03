#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <fstream>
#include <chrono>

using namespace std;
void createRandomSystem(vector<vector<double>>& coeffs, vector<double>& constants, int size) {
    random_device seed;
    mt19937 generator(seed());
    uniform_real_distribution<double> value_dist(0, 100.0);
    uniform_real_distribution<double> value_spec(101.0, 200.0);

    coeffs.resize(size, vector<double>(size));
    constants.resize(size);

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            coeffs[row][col] = value_dist(generator);
            if(col == row){
                coeffs[row][col] = value_spec(generator);
            }
        }
        constants[row] = value_dist(generator);
    }
}
// Проверка на сходимость (диагональное преобладание)
bool isConvergent(const vector<vector<double>>& A) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        double sum = 0.0;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                sum += fabs(A[i][j]);
            }
        }
        if (fabs(A[i][i]) <= sum) {
            return false;
        }
    }
    return true;
}

// Метод простых итераций (Якоби)
vector<double> solveJacobi(const vector<vector<double>>& A, const vector<double>& b, double eps = 1e-6, int maxIter = 1000) {
    int n = A.size();
    vector<double> x(n, 0.0); // Начальное приближение (нули)
    vector<double> x_new(n);

    int iter = 0;
    double error;

    do {
        error = 0.0;
        for (int i = 0; i < n; ++i) {
            x_new[i] = b[i];
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    x_new[i] -= A[i][j] * x[j];
                }
            }
            x_new[i] /= A[i][i];
            error += fabs(x_new[i] - x[i]);
        }
        x = x_new;
        iter++;
    } while (error > eps && iter < maxIter);

    if (iter == maxIter) {
        cout << "Достигнуто максимальное число итераций!" << endl;
    }

    cout << "Количество итераций: " << iter << endl;
    return x;
}

int main() {
    vector<vector<double>> A;
    vector<double> b;
    int n; cin>>n;
    createRandomSystem(A, b, n);
    if (!isConvergent(A)) {
        cout << "Матрица не имеет диагонального преобладания. Метод может не сойтись!" << endl;
    }

    vector<double> solution = solveJacobi(A, b);
    cout.precision(6);
    cout << "Решение системы:" << endl;
    for (size_t i = 0; i < solution.size(); ++i) {
        cout << "x[" << i << "] = " << solution[i] << endl;
    }

    return 0;
}