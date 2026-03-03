#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <iomanip>
using namespace std;
int PARALLEL_THRESHOLD = 16; // Порог для параллельной сортировки
class ParallelSorter {
private:
static void parallelQuickSortRecursive(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    
    int size = right - left + 1;
    
    if (size < PARALLEL_THRESHOLD) {
        sort(arr.begin() + left, arr.begin() + right + 1);
        return;
    }
    
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
    
    int pivot = arr[mid];
    swap(arr[mid], arr[right - 1]);
    
    int i = left + 1;
    int j = right - 2;
    
    while (true) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i >= j) break;
        swap(arr[i], arr[j]);
        i++;
        j--;
    }
    
    swap(arr[i], arr[right - 1]);
    int partitionIndex = i;
    
    auto future_left = async(launch::async, [&arr, left, partitionIndex]() {
        parallelQuickSortRecursive(arr, left, partitionIndex - 1);
    });
    
    auto future_right = async(launch::async, [&arr, partitionIndex, right]() {
        parallelQuickSortRecursive(arr, partitionIndex + 1, right);
    });
    
    future_left.get();
    future_right.get();
}
    
    static void sequentialQuickSortRecursive(vector<int>& arr, int left, int right) {
        if (left >= right) return;
        
        int size = right - left + 1;
        
        if (size < PARALLEL_THRESHOLD) {
            sort(arr.begin() + left, arr.begin() + right + 1);
            return;
        }
        
        int mid = left + (right - left) / 2;
        if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
        if (arr[left] > arr[right]) swap(arr[left], arr[right]);
        if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
        
        int pivot = arr[mid];
        swap(arr[mid], arr[right - 1]);
        
        int i = left + 1;
        int j = right - 2;
        
        while (true) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i >= j) break;
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
        
        swap(arr[i], arr[right - 1]);
        int partitionIndex = i;
        
        sequentialQuickSortRecursive(arr, left, partitionIndex - 1);
        sequentialQuickSortRecursive(arr, partitionIndex + 1, right);
    }

public:
    static vector<int> parallelQuickSort(const vector<int>& arr) {
        vector<int> result = arr;
        parallelQuickSortRecursive(result, 0, result.size() - 1);
        return result;
    }
    
    static vector<int> sequentialQuickSort(const vector<int>& arr) {
        vector<int> result = arr;
        sequentialQuickSortRecursive(result, 0, result.size() - 1);
        return result;
    }
    
    static pair<vector<int>, double> parallelQuickSortWithTime(const vector<int>& arr) {
        auto start = chrono::high_resolution_clock::now();
        vector<int> result = parallelQuickSort(arr);
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration = end - start;
        return {result, duration.count()};
    }
    
    static pair<vector<int>, double> sequentialQuickSortWithTime(const vector<int>& arr) {
        auto start = chrono::high_resolution_clock::now();
        vector<int> result = sequentialQuickSort(arr);
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration = end - start;
        return {result, duration.count()};
    }
    
    static vector<int> generateRandomArray(int size, int minVal = 0, int maxVal = 1000000) {
        vector<int> arr(size);
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> dist(minVal, maxVal);
        
        for (int i = 0; i < size; i++) {
            arr[i] = dist(rng);
        }
        
        return arr;
    }
    
    static bool isSorted(const vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); i++) {
            if (arr[i] < arr[i-1]) {
                return false;
            }
        }
        return true;
    }
    
    static bool areEqual(const vector<int>& arr1, const vector<int>& arr2) {
        if (arr1.size() != arr2.size()) return false;
        for (size_t i = 0; i < arr1.size(); i++) {
            if (arr1[i] != arr2[i]) {
                return false;
            }
        }
        return true;
    }
};

void testSortingCorrectness(int maxSize = 100000, int numTests = 10) {
    cout << "\nТЕСТИРОВАНИЕ КОРРЕКТНОСТИ СОРТИРОВКИ" << endl;
    cout << "====================================" << endl;
    
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distSize(1000, maxSize);
    
    int passed = 0;
    
    for (int test = 0; test < numTests; test++) {
        int size = distSize(rng);
        PARALLEL_THRESHOLD = size/12;
        auto arr = ParallelSorter::generateRandomArray(size, 0, 100000);
        
        vector<int> stdSorted = arr;
        auto start_std = chrono::high_resolution_clock::now();
        sort(stdSorted.begin(), stdSorted.end());
        auto end_std = chrono::high_resolution_clock::now();
        double time_std = chrono::duration<double>(end_std - start_std).count() * 1000;
        
        bool stdSortedCorrect = ParallelSorter::isSorted(stdSorted);
        
        auto start_seq = chrono::high_resolution_clock::now();
        auto seqResult = ParallelSorter::sequentialQuickSort(arr);
        auto end_seq = chrono::high_resolution_clock::now();
        double time_seq = chrono::duration<double>(end_seq - start_seq).count() * 1000;
        
        bool seqSorted = ParallelSorter::isSorted(seqResult);
        
        auto start_par = chrono::high_resolution_clock::now();
        auto parResult = ParallelSorter::parallelQuickSort(arr);
        auto end_par = chrono::high_resolution_clock::now();
        double time_par = chrono::duration<double>(end_par - start_par).count() * 1000;
        
        bool parSorted = ParallelSorter::isSorted(parResult);
        
        bool seqMatchesStd = ParallelSorter::areEqual(seqResult, stdSorted);
        bool parMatchesStd = ParallelSorter::areEqual(parResult, stdSorted);
        bool ourImplementationsMatch = ParallelSorter::areEqual(seqResult, parResult);
        
        if (stdSortedCorrect && seqSorted && parSorted && 
            seqMatchesStd && parMatchesStd && ourImplementationsMatch) {
            passed++;
        } else {
            cout << "Тест " << test + 1 << " НЕ ПРОЙДЕН! Размер: " << size 
                 << " (std::sort: " << fixed << setprecision(2) << time_std 
                 << "ms, seq: " << time_seq << "ms, par: " << time_par << "ms)" << endl;
            if (!stdSortedCorrect) cout << "  - std::sort не отсортировал" << endl;
            if (!seqSorted) cout << "  - sequentialQuickSort не отсортировал" << endl;
            if (!parSorted) cout << "  - parallelQuickSort не отсортировал" << endl;
            if (!seqMatchesStd) cout << "  - sequentialQuickSort != std::sort" << endl;
            if (!parMatchesStd) cout << "  - parallelQuickSort != std::sort" << endl;
            if (!ourImplementationsMatch) cout << "  - sequentialQuickSort != parallelQuickSort" << endl;
        }
    }
    
    cout << "Пройдено тестов: " << passed << "/" << numTests << endl;
    cout << "Корректность: " << (passed == numTests ? "✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ" : "✗ ЕСТЬ ОШИБКИ") << endl;
}

void performanceComparisonSorting(int minSize = 10000, int maxSize = 1000000, int step = 5, int numTrials = 5) {
    cout << "\nСРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ СОРТИРОВКИ" << endl;
    cout << "=======================================" << endl;
    cout << "Размер  | SeqQuick(ms) | ParQuick(ms) | Ускорение" << endl;
    cout << "--------|-------------|-------------|-----------" << endl;
    
    for (int size = minSize; size <= maxSize; size += minSize*4) {
        cout << setw(7) << size << " | ";
        PARALLEL_THRESHOLD = size/12;
        auto arr = ParallelSorter::generateRandomArray(size, 0, 1000000);
        
        double timeSeq = 0, timePar = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            auto arrCopy = arr;
            auto seqResult = ParallelSorter::sequentialQuickSortWithTime(arrCopy);
            timeSeq += seqResult.second * 1000;
            
            arrCopy = arr;
            auto parResult = ParallelSorter::parallelQuickSortWithTime(arrCopy);
            timePar += parResult.second * 1000;
        }
        
        timeSeq /= numTrials;
        timePar /= numTrials;
        
        double speedup = timeSeq / timePar;
        
        cout << fixed << setprecision(2)
             << setw(11) << timeSeq << " | "
             << setw(11) << timePar << " | "
             << setw(9) << setprecision(2) << speedup << endl;
    }
}
