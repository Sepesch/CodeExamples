#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <cassert>
#include <iomanip>
using namespace std;
class Matrix {
private:
    vector<vector<int>> data;
    int rows, cols;

public:
    Matrix(int r, int c): rows(r), cols(c), data(r, vector<int>(c, 0)) {}
    Matrix(const vector<vector<int>>& d): data(d), rows(d.size()), cols(d[0].size()) {}

    int getRows() const {return rows;} int getCols() const{return cols;}
    int& operator()(int i, int j){return data[i][j];}
    const int& operator()(int i, int j) const {return data[i][j];}
    void randomInit(int min = 0, int max = 100){
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                data[i][j] = min + rand()%(max-min);
            }
        }
    }
    bool operator==(const Matrix& other){
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
    void print() const {
        for (int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                cout<<data[i][j]<<" ";
            }
            cout<<endl;
        }
    }
};

void multiplyBlock(const Matrix& A, const Matrix& B, Matrix& result,
                    int startR, int endR){
    int m=B.getRows();
    int l=B.getCols();
    int sum;
    for(int i=startR; i<endR; i++){
        for(int j=0; j<m; j++){
            for(int k=0; k<l; k++){
                result(i, j) +=A(i, k) * B(k, j);
            }
        }
    }
}
//N - number of threads
Matrix threadedMatrixMulti(const Matrix& A, const Matrix& B, int N){
    assert(A.getCols() == B.getCols());
    int n = A.getRows(); int p=B.getCols();
    Matrix res(n, p);
    vector<thread> threads;

    int RPerThread = n / N;
    int extraR = n % N;
    
    int startR = 0;
    for(int t=0; t<N; t++){
        int endR = startR + RPerThread + (t<extraR ? 1: 0);
        threads.emplace_back([&A, &B, &res, startR, endR](){
            multiplyBlock(A, B, res, startR, endR);
        });
        startR += RPerThread + (t<extraR ? 1: 0);
    }
    for (auto& thread : threads){
       thread.join();
    }

    return res;
}

Matrix asyncedMatrixMulti (const Matrix& A, const Matrix& B, int N){
    assert(A.getCols() == B.getCols());
    int n = A.getRows(); int p=B.getCols();
    Matrix res(n, p);
    vector<future<void>> futures;

    int RPerThread = n / N;
    int extraR = n % N;
    
    int startR = 0;
    for(int t=0; t<N; t++){
        int endR = startR + RPerThread + (t<extraR ? 1: 0);
        futures.emplace_back(async(launch::async, [&A, &B, &res, startR, endR](){
            multiplyBlock(A, B, res, startR, endR);}));
            startR += RPerThread + (t<extraR ? 1: 0);
    }
    for (auto& future : futures){
       future.get();
    }

    return res;
}

template<typename Func, typename... Args>
double measureExecTime(Func&& func, Args&&... args){
    auto start = chrono::high_resolution_clock::now();
    forward<Func>(func)(forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();

    return chrono::duration<double, milli>(end-start).count();
}

void performanceResearch(int minSize, int maxSize, int step,
                        int minThreads, int maxThreads, int numTrials=5){
    cout<<"Исследование производительности"<<endl;
    cout<<"Size\tThreads\tthreadTime(ms)\tasyncTime(ms)\tSpeedup\tEfficiency"<<endl;

    for(int size = minSize; size <=maxSize; size += step){
        Matrix A(size, size);
        Matrix B(size, size);
        Matrix res(size, size);
        double singleTime = 0;
        for(int trial = 0; trial < numTrials; trial++){
            singleTime += measureExecTime(multiplyBlock, A, B, res, 0, size);
        }
        singleTime /= numTrials;

        for (int numThreads = minThreads; numThreads <= maxThreads; numThreads++){
            double threadsTime=0, asyncTime=0;
            for(int trial = 0; trial <numTrials; trial++){
                threadsTime +=measureExecTime(threadedMatrixMulti, A, B, numThreads);
                asyncTime += measureExecTime(asyncedMatrixMulti, A, B, numThreads);
            }
            threadsTime /= numTrials;
            asyncTime /= numTrials;
            double speedup = singleTime / threadsTime;
            double effinciency = speedup / numThreads;

            cout<<size<<"\t"<<numThreads<<"\t"<<fixed<<setprecision(2)
                <<threadsTime<<"\t\t"<<asyncTime<<"\t\t"<<speedup<<"\t"
                <<effinciency<<endl;
        }
    }
}

void findOptimalThreadCount(int matrixSize = 500, int maxThreads = 16, int numTrials = 5){
    cout<<"Finding optimal number of threads for matrix size "<<matrixSize<<"x"<<matrixSize<<endl;
    Matrix A(matrixSize, matrixSize); Matrix B(matrixSize, matrixSize);
    A.randomInit(); B.randomInit();

    double bestTime = numeric_limits<double>::max();
    int optimalThreads = 1;

    for(int numThreads = 20; numThreads <= maxThreads; numThreads++){
        double totalTime = 0;

        for(int trial = 0; trial < numTrials; trial++){
                totalTime += measureExecTime(threadedMatrixMulti, A, B, numThreads);
        }

        double avgTime = totalTime / numTrials;
        cout<<"Threads: "<<numThreads<<" - Time: "<<avgTime<<"ms"<<endl;

        if(avgTime < bestTime){bestTime = avgTime; optimalThreads = numThreads;}
    }

    cout<<"Optimal number of threads: "<<optimalThreads<<" with time: "<<bestTime<<"ms"<<endl;
}

int main(){
    cout<<"Threaded Matrix Multiplication Tests"<<endl;
    cout<<"===================================="<<endl;

    cout<<"Checking the correction of calculation of threaded and async\n";
    Matrix A(10, 10), B(10,10);
    A.randomInit(); B.randomInit();

    Matrix res1 = threadedMatrixMulti(A, B, 1);
    Matrix res2 = threadedMatrixMulti(A, B, 10);
    Matrix res3 = asyncedMatrixMulti(A, B, 10);
    std::cout << "Compare result thread: " << (res1==res2 ? "OK" : "FAIL") << std::endl;
    std::cout << "Compare result async: " << (res1==res3 ? "OK" : "FAIL") << std::endl;


    findOptimalThreadCount(500, 30, 5);
    cout<<endl;

    performanceResearch(100, 500, 100, 1, 16);

    return 0;
}