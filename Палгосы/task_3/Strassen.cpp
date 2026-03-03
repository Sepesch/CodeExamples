#include "Matrix.cpp"
#include <vector>
#include <future>
#include <chrono>
#include <cassert>
#include <algorithm>
using namespace std;

class StrassenMultiplier {
private:
    static const int TRIVIAL_BOUND = 32; // Оптимальный порог для переключения
    
    static vector<vector<int>> add(const vector<vector<int>>& a, 
                                   const vector<vector<int>>& b) {
        int n = a.size();
        vector<vector<int>> result(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i][j] = a[i][j] + b[i][j];
        return result;
    }

    static vector<vector<int>> sub(const vector<vector<int>>& a, 
                                   const vector<vector<int>>& b) {
        int n = a.size();
        vector<vector<int>> result(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i][j] = a[i][j] - b[i][j];
        return result;
    }

    static vector<vector<int>> trivialMul(const vector<vector<int>>& a, 
                                          const vector<vector<int>>& b) {
        int n = a.size();
        vector<vector<int>> result(n, vector<int>(n, 0));
        
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                int aik = a[i][k];
                for (int j = 0; j < n; j++) {
                    result[i][j] += aik * b[k][j];
                }
            }
        }
        return result;
    }

    static vector<vector<int>> strassenRecursive(const vector<vector<int>>& a,
                                                 const vector<vector<int>>& b) {
        int n = a.size();
        
        if (n <= TRIVIAL_BOUND) {
            return trivialMul(a, b);
        }
        
        return strassen2x2(a, b);
    }

    static vector<vector<int>> strassen2x2(const vector<vector<int>>& a,
                                           const vector<vector<int>>& b) {
        int n = a.size();
        int h = n / 2;
        
        vector<vector<int>> a11(h, vector<int>(h));
        vector<vector<int>> a12(h, vector<int>(h));
        vector<vector<int>> a21(h, vector<int>(h));
        vector<vector<int>> a22(h, vector<int>(h));
        
        vector<vector<int>> b11(h, vector<int>(h));
        vector<vector<int>> b12(h, vector<int>(h));
        vector<vector<int>> b21(h, vector<int>(h));
        vector<vector<int>> b22(h, vector<int>(h));
        
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < h; j++) {
                a11[i][j] = a[i][j];
                a12[i][j] = a[i][j+h];
                a21[i][j] = a[i+h][j];
                a22[i][j] = a[i+h][j+h];
                
                b11[i][j] = b[i][j];
                b12[i][j] = b[i][j+h];
                b21[i][j] = b[i+h][j];
                b22[i][j] = b[i+h][j+h];
            }
        }
        
        // Асинхронное вычисление промежуточных матриц
        auto future_M1 = async(launch::async, [&]() { return strassenRecursive(add(a11, a22), add(b11, b22)); });
        auto future_M2 = async(launch::async, [&]() { return strassenRecursive(add(a21, a22), b11); });
        auto future_M3 = async(launch::async, [&]() { return strassenRecursive(a11, sub(b12, b22)); });
        auto future_M4 = async(launch::async, [&]() { return strassenRecursive(a22, sub(b21, b11)); });
        auto future_M5 = async(launch::async, [&]() { return strassenRecursive(add(a11, a12), b22); });
        auto future_M6 = async(launch::async, [&]() { return strassenRecursive(sub(a21, a11), add(b11, b12)); });
        auto future_M7 = async(launch::async, [&]() { return strassenRecursive(sub(a12, a22), add(b21, b22)); });
        
        vector<vector<int>> M1 = future_M1.get();
        vector<vector<int>> M2 = future_M2.get();
        vector<vector<int>> M3 = future_M3.get();
        vector<vector<int>> M4 = future_M4.get();
        vector<vector<int>> M5 = future_M5.get();
        vector<vector<int>> M6 = future_M6.get();
        vector<vector<int>> M7 = future_M7.get();
        
        vector<vector<int>> result(n, vector<int>(n));
        
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < h; j++) {
                result[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
                result[i][j+h] = M3[i][j] + M5[i][j];
                result[i+h][j] = M2[i][j] + M4[i][j];
                result[i+h][j+h] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
            }
        }
        
        return result;
    }

public:
    static Matrix multiply(const Matrix& A, const Matrix& B) {
        assert(A.getCols() == B.getRows());
        int n = A.getRows();
        int m = B.getCols();
        int k = A.getCols();
        
        int maxSize = max({n, m, k});
        int size = 1;
        while (size < maxSize) size *= 2;
        
        vector<vector<int>> a(size, vector<int>(size, 0));
        vector<vector<int>> b(size, vector<int>(size, 0));
        
        for (int i = 0; i < n; i++)
            for (int j = 0; j < k; j++)
                a[i][j] = A(i, j);
                
        for (int i = 0; i < k; i++)
            for (int j = 0; j < m; j++)
                b[i][j] = B(i, j);
        
        auto result = strassenRecursive(a, b);
        
        Matrix C(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                C(i, j) = result[i][j];
        
        return C;
    }
    
    static pair<Matrix, double> multiplyWithTime(const Matrix& A, const Matrix& B) {
        auto start = chrono::high_resolution_clock::now();
        Matrix result = multiply(A, B);
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration = end - start;
        return {result, duration.count()};
    }
};

// Функция для измерения времени выполнения
template<typename Func, typename... Args>
double measureExecTime(Func&& func, Args&&... args){
    auto start = chrono::high_resolution_clock::now();
    forward<Func>(func)(forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end-start).count();
}

// Функция тестирования корректности
void testCorrectness(int maxSize = 64, int numTests = 10) {
    cout << "\nТЕСТИРОВАНИЕ КОРРЕКТНОСТИ" << endl;
    cout << "==========================" << endl;
    
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, 10);
    
    int passed = 0;
    
    for (int test = 0; test < numTests; test++) {
        int size = dist(rng) % maxSize + 1;
        
        Matrix A(size, size);
        Matrix B(size, size);
        A.randomInit(0, 10);
        B.randomInit(0, 10);
        
        // Базовое умножение (1 поток)
        Matrix baseResult = threadedMatrixMulti(A, B, 1);
        
        // Умножение Штрассена
        Matrix strassenResult = StrassenMultiplier::multiply(A, B);
        
        if (baseResult == strassenResult) {
            passed++;
        } else {
            cout << "Тест " << test + 1 << " НЕ ПРОЙДЕН! Размер: " << size << "x" << size << endl;
        }
    }
    
    cout << "Пройдено тестов: " << passed << "/" << numTests << endl;
    cout << "Корректность: " << (passed == numTests ? "✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ" : "✗ ЕСТЬ ОШИБКИ") << endl;
}

// Функция сравнения производительности
void performanceComparison(int minSize = 64, int maxSize = 2048, int step = 2, int numTrials = 5) {
    cout << "\nСРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ" << endl;
    cout << "=============================" << endl;
    cout << "Размер | Threads (12) | Async (12) | Strassen | Ускорение" << endl;
    cout << "-------|-------------|-----------|----------|----------" << endl;
    
    int optimalThreads = thread::hardware_concurrency();
    
    for (int size = minSize; size <= maxSize; size *= step) {
        cout << setw(6) << size << " | ";
        
        Matrix A(size, size);
        Matrix B(size, size);
        A.randomInit(0, 10);
        B.randomInit(0, 10);
        
        double timeThreadsN = 0, timeAsyncN = 0, timeStrassen = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            timeThreadsN += measureExecTime(threadedMatrixMulti, A, B, optimalThreads);
            timeAsyncN += measureExecTime(asyncedMatrixMulti, A, B, optimalThreads);
            
            auto result = StrassenMultiplier::multiplyWithTime(A, B);
            timeStrassen += result.second * 1000;
        }
        
        timeThreadsN /= numTrials;
        timeAsyncN /= numTrials;
        timeStrassen /= numTrials;
        
        double bestLR1Time = min({timeThreadsN, timeAsyncN});
        double speedup = bestLR1Time / timeStrassen;
        
        cout << fixed << setprecision(2)
             << setw(11) << timeThreadsN << " | "
             << setw(9) << timeAsyncN << " | "
             << setw(8) << timeStrassen << " | "
             << setw(9) << setprecision(2) << speedup << endl;
    }
    
    cout << "\nОптимальное число потоков (auto-detected): " << optimalThreads << endl;
}
