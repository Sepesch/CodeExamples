#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <CL/cl.h>
#include "Matrix.cpp"

class GPUMatrixMultiplier {
private:
    cl_context context;
    cl_command_queue queue;
    cl_kernel kernel;
    cl_program program;
    
public:
    GPUMatrixMultiplier() {
        cl_platform_id platform;
        cl_device_id device;
        cl_int err;
        
        err = clGetPlatformIDs(1, &platform, NULL);
        
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
        if (err != CL_SUCCESS) {
            err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
        }
        
        context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
        queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
        
        const char* kernelSource = R"(
            __kernel void multiply(__global const int* A, __global const int* B, 
                                   __global int* C, int n, int m, int p) {
                int i = get_global_id(0);
                int j = get_global_id(1);
                
                if (i < n && j < m) {
                    int sum = 0;
                    for (int k = 0; k < p; k++) {
                        sum += A[i * p + k] * B[k * m + j];
                    }
                    C[i * m + j] = sum;
                }
            }
        )";
        
        program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
        err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
        
        // Создаем ядро
        kernel = clCreateKernel(program, "multiply", &err);
    }
    
    ~GPUMatrixMultiplier() {
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
    }
    
    std::vector<int> multiply(const std::vector<int>& A, const std::vector<int>& B, 
                              int n, int p, int m) {
        cl_int err;
        std::vector<int> C(n * m);
        
        // Создаем буферы
        cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                                       n * p * sizeof(int), (void*)A.data(), &err);
        cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                       p * m * sizeof(int), (void*)B.data(), &err);
        cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
                                       n * m * sizeof(int), NULL, &err);
        
        // Устанавливаем аргументы
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
        clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
        clSetKernelArg(kernel, 3, sizeof(int), &n);
        clSetKernelArg(kernel, 4, sizeof(int), &m);
        clSetKernelArg(kernel, 5, sizeof(int), &p);
        
        size_t global[2] = {static_cast<size_t>(m), static_cast<size_t>(n)};
        clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global, NULL, 0, NULL, NULL);
        
        clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, 
                           n * m * sizeof(int), C.data(), 0, NULL, NULL);
        
        clReleaseMemObject(bufferA);
        clReleaseMemObject(bufferB);
        clReleaseMemObject(bufferC);
        
        return C;
    }
    
    // Метод для умножения Matrix объектов
    Matrix multiplyMatrix(const Matrix& A, const Matrix& B) {
        int n = A.getRows();
        int p = A.getCols();
        int m = B.getCols();
        
        // Преобразуем матрицы в плоские векторы
        std::vector<int> flatA(n * p);
        std::vector<int> flatB(p * m);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < p; j++) {
                flatA[i * p + j] = A(i, j);
            }
        }
        
        for (int i = 0; i < p; i++) {
            for (int j = 0; j < m; j++) {
                flatB[i * m + j] = B(i, j);
            }
        }
        
        // Умножаем на GPU
        std::vector<int> flatC = multiply(flatA, flatB, n, p, m);
        
        // Преобразуем результат обратно в Matrix
        Matrix result(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result(i, j) = flatC[i * m + j];
            }
        }
        
        return result;
    }
};

// Последовательное умножение для сравнения
Matrix sequentialMatrixMulti(const Matrix& A, const Matrix& B) {
    assert(A.getCols() == B.getRows());
    int n = A.getRows();
    int m = B.getCols();
    int p = A.getCols();
    Matrix result(n, m);
    
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int sum = 0;
            for(int k=0; k<p; k++){
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

void testMatrixMultiplication() {
    std::cout << "=== Блочное умножение матриц (4.1) ===" << std::endl;
    std::cout << "Сравнение CPU (ЛР1) vs GPU (OpenCL)\n" << std::endl;
    
    GPUMatrixMultiplier gpu;
    
    // Разные размеры для тестирования
    std::vector<int> sizes = {64, 128, 256, 512, 1024, 1536, 2048, 2560, 3072};
    int threads = 12; 
    
    std::cout << std::setw(10) << "Размер " 
              << std::setw(15) << "CPU (12 потоков) "
              << std::setw(15) << "GPU OpenCL "
              << std::setw(15) << "Ускорение "
              << std::setw(15) << "Корректность" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (int size : sizes) {
        Matrix A(size, size);
        Matrix B(size, size);
        A.randomInit(0, 100);
        B.randomInit(0, 100);
        
        auto start = std::chrono::high_resolution_clock::now();
        Matrix cpuResult = threadedMatrixMulti(A, B, threads);
        auto end = std::chrono::high_resolution_clock::now();
        auto cpuTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        start = std::chrono::high_resolution_clock::now();
        Matrix gpuResult = gpu.multiplyMatrix(A, B);
        end = std::chrono::high_resolution_clock::now();
        auto gpuTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        bool correct = (cpuResult == gpuResult);
        double speedup = (double)cpuTime / gpuTime;
        
        std::cout << std::setw(10) << size << "x" << size
                  << std::setw(15) << cpuTime << " мс"
                  << std::setw(15) << gpuTime << " мс"
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup << "x"
                  << std::setw(15) << (correct ? "Да" : "НЕТ!") << std::endl;
    }
}