#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <cassert>
#include <iomanip>
#include <random>
using namespace std;
class Matrix {
private:
    vector<vector<int>> data;
    int rows, cols;

public:
    Matrix(int r, int c): rows(r), cols(c), data(r, vector<int>(c, 0)) {}
    Matrix(const vector<vector<int>>& d): data(d), rows(d.size()), cols(d[0].size()) {}

    int getRows() const {return rows;} 
    int getCols() const {return cols;}
    
    int& operator()(int i, int j){return data[i][j];}
    const int& operator()(int i, int j) const {return data[i][j];}
    
    void randomInit(int min = 0, int max = 100){
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> dist(min, max);
        
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                data[i][j] = dist(rng);
            }
        }
    }
    
    bool operator==(const Matrix& other) const {
        if(rows != other.rows || cols != other.cols) return false;
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                if(data[i][j] != other.data[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }
    
    void print() const {
        for (int i=0; i<min(rows, 5); i++){
            for(int j=0; j<min(cols, 5); j++){
                cout<<data[i][j]<<" ";
            }
            if(cols > 5) cout << "...";
            cout<<endl;
        }
        if(rows > 5) cout << "... (" << rows << "x" << cols << " matrix)" << endl;
    }
    
    vector<vector<int>> getData() const { return data; }
};

// ==================== ФУНКЦИИ ИЗ ЛР1 ====================
void multiplyBlock(const Matrix& A, const Matrix& B, Matrix& result,
                    int startR, int endR){
    int n = B.getRows();
    int m = B.getCols();
    for(int i=startR; i<endR; i++){
        for(int j=0; j<m; j++){
            int sum = 0;
            for(int k=0; k<n; k++){
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }
}

Matrix threadedMatrixMulti(const Matrix& A, const Matrix& B, int N){
    assert(A.getCols() == B.getRows());
    int n = A.getRows(); 
    int m = B.getCols();
    Matrix res(n, m);
    vector<thread> threads;

    int rowsPerThread = n / N;
    int extraRows = n % N;
    
    int startRow = 0;
    for(int t=0; t<N; t++){
        int endRow = startRow + rowsPerThread + (t<extraRows ? 1: 0);
        threads.emplace_back([&A, &B, &res, startRow, endRow](){
            multiplyBlock(A, B, res, startRow, endRow);
        });
        startRow = endRow;
    }
    for (auto& thread : threads){
       thread.join();
    }

    return res;
}

Matrix asyncedMatrixMulti(const Matrix& A, const Matrix& B, int N){
    assert(A.getCols() == B.getRows());
    int n = A.getRows(); 
    int m = B.getCols();
    Matrix res(n, m);
    vector<future<void>> futures;

    int rowsPerThread = n / N;
    int extraRows = n % N;
    
    int startRow = 0;
    for(int t=0; t<N; t++){
        int endRow = startRow + rowsPerThread + (t<extraRows ? 1: 0);
        futures.emplace_back(async(launch::async, [&A, &B, &res, startRow, endRow](){
            multiplyBlock(A, B, res, startRow, endRow);}));
        startRow = endRow;
    }
    for (auto& future : futures){
       future.get();
    }

    return res;
}