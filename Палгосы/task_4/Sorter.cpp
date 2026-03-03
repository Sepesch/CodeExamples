#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <chrono>
#include <cstring>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

// ==================== CPU Сортировка (простая многопоточная) ====================

// Слияние двух отсортированных массивов
static void merge(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int* L = new int[n1];
    int* R = new int[n2];
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

// Последовательная сортировка слиянием
static void mergeSortSeq(int* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSeq(arr, left, mid);
        mergeSortSeq(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Параллельная сортировка слиянием
static void mergeSortParallel(int* arr, int left, int right, int depth = 0) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    
    // Распараллеливаем только первые 2 уровня
    if (depth < 2 && (right - left) > 1000) {
        auto left_future = std::async(std::launch::async,
            [arr, left, mid, depth]() {
                mergeSortParallel(arr, left, mid, depth + 1);
            });
        
        mergeSortParallel(arr, mid + 1, right, depth + 1);
        left_future.wait();
    } else {
        mergeSortSeq(arr, left, mid);
        mergeSortSeq(arr, mid + 1, right);
    }
    
    merge(arr, left, mid, right);
}

// Публичная функция CPU сортировки
extern "C" void mergeSortCPU(int* data, int size) {
    if (size <= 1) return;
    mergeSortParallel(data, 0, size - 1);
}

// ==================== GPU Сортировка (OpenCL) ====================

// Простая проверка ошибок OpenCL
static bool checkCL(cl_int err, const char* msg) {
    if (err != CL_SUCCESS) {
        std::cerr << "OpenCL ошибка (" << msg << "): " << err << std::endl;
        return false;
    }
    return true;
}

// Публичная функция GPU сортировки
extern "C" bool mergeSortGPU(int* data, int size) {
    if (size <= 1) return true;
    
    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel_merge, kernel_copy;
    
    // 1. Получаем платформу
    err = clGetPlatformIDs(1, &platform, nullptr);
    if (!checkCL(err, "clGetPlatformIDs")) return false;
    
    // 2. Получаем устройство (сначала GPU, потом CPU)
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (err != CL_SUCCESS) {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, nullptr);
        if (!checkCL(err, "clGetDeviceIDs")) return false;
    }
    
    // 3. Создаем контекст
    context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    if (!checkCL(err, "clCreateContext")) return false;
    
    // 4. Создаем очередь команд
    queue = clCreateCommandQueue(context, device, 0, &err);
    if (!checkCL(err, "clCreateCommandQueue")) return false;
    
    // 5. Создаем программу с встроенным ядром
    const char* kernel_source = R"(
__kernel void merge_sort(__global int* array, 
                         __global int* temp, 
                         int width,
                         int size) {
    int idx = get_global_id(0);
    int start = idx * width * 2;
    
    if (start >= size) return;
    
    int mid = min(start + width, size);
    int end = min(start + width * 2, size);
    
    int i = start, j = mid, k = start;
    
    while (i < mid && j < end) {
        if (array[i] <= array[j]) {
            temp[k++] = array[i++];
        } else {
            temp[k++] = array[j++];
        }
    }
    
    while (i < mid) temp[k++] = array[i++];
    while (j < end) temp[k++] = array[j++];
}

__kernel void copy_back(__global int* src, 
                        __global int* dst, 
                        int size) {
    int idx = get_global_id(0);
    if (idx < size) dst[idx] = src[idx];
}
)";
    
    program = clCreateProgramWithSource(context, 1, &kernel_source, nullptr, &err);
    if (!checkCL(err, "clCreateProgramWithSource")) return false;
    
    // 6. Компилируем программу
    err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
        char* log = new char[log_size];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, nullptr);
        std::cerr << "Ошибка компиляции OpenCL:\n" << log << std::endl;
        delete[] log;
        return false;
    }
    
    // 7. Создаем ядра
    kernel_merge = clCreateKernel(program, "merge_sort", &err);
    if (!checkCL(err, "clCreateKernel merge_sort")) return false;
    
    kernel_copy = clCreateKernel(program, "copy_back", &err);
    if (!checkCL(err, "clCreateKernel copy_back")) return false;
    
    // 8. Создаем буферы
    size_t buffer_size = size * sizeof(int);
    cl_mem buf_data = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
                                     buffer_size, data, &err);
    if (!checkCL(err, "clCreateBuffer data")) return false;
    
    cl_mem buf_temp = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, nullptr, &err);
    if (!checkCL(err, "clCreateBuffer temp")) return false;
    
    // 9. Выполняем сортировку
    int width = 1;
    while (width < size) {
        // Настраиваем ядро слияния
        err = clSetKernelArg(kernel_merge, 0, sizeof(cl_mem), &buf_data);
        err |= clSetKernelArg(kernel_merge, 1, sizeof(cl_mem), &buf_temp);
        err |= clSetKernelArg(kernel_merge, 2, sizeof(int), &width);
        err |= clSetKernelArg(kernel_merge, 3, sizeof(int), &size);
        
        if (!checkCL(err, "clSetKernelArg merge")) {
            clReleaseMemObject(buf_data);
            clReleaseMemObject(buf_temp);
            return false;
        }
        
        // Запускаем ядро слияния
        size_t global_size = (size + 2 * width - 1) / (2 * width);
        err = clEnqueueNDRangeKernel(queue, kernel_merge, 1, nullptr, 
                                     &global_size, nullptr, 0, nullptr, nullptr);
        if (!checkCL(err, "clEnqueueNDRangeKernel merge")) {
            clReleaseMemObject(buf_data);
            clReleaseMemObject(buf_temp);
            return false;
        }
        
        // Настраиваем ядро копирования
        err = clSetKernelArg(kernel_copy, 0, sizeof(cl_mem), &buf_temp);
        err |= clSetKernelArg(kernel_copy, 1, sizeof(cl_mem), &buf_data);
        err |= clSetKernelArg(kernel_copy, 2, sizeof(int), &size);
        
        if (!checkCL(err, "clSetKernelArg copy")) {
            clReleaseMemObject(buf_data);
            clReleaseMemObject(buf_temp);
            return false;
        }
        
        // Запускаем ядро копирования
        size_t copy_size = size;
        err = clEnqueueNDRangeKernel(queue, kernel_copy, 1, nullptr, 
                                     &copy_size, nullptr, 0, nullptr, nullptr);
        if (!checkCL(err, "clEnqueueNDRangeKernel copy")) {
            clReleaseMemObject(buf_data);
            clReleaseMemObject(buf_temp);
            return false;
        }
        
        width *= 2;
    }
    
    // 10. Читаем результат
    err = clEnqueueReadBuffer(queue, buf_data, CL_TRUE, 0, 
                              buffer_size, data, 0, nullptr, nullptr);
    if (!checkCL(err, "clEnqueueReadBuffer")) {
        clReleaseMemObject(buf_data);
        clReleaseMemObject(buf_temp);
        return false;
    }
    
    // 11. Очищаем ресурсы
    clReleaseMemObject(buf_data);
    clReleaseMemObject(buf_temp);
    clReleaseKernel(kernel_merge);
    clReleaseKernel(kernel_copy);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    
    return true;
}

// ==================== Вспомогательные функции ====================

// Проверка отсортированности
extern "C" bool isSorted(int* data, int size) {
    for (int i = 1; i < size; i++) {
        if (data[i] < data[i - 1]) return false;
    }
    return true;
}

// Стандартная сортировка для сравнения
extern "C" void stdSort(int* data, int size) {
    std::sort(data, data + size);
}

// Генерация случайного массива
extern "C" void generateRandomArray(int* data, int size, int max_value = 1000000) {
    std::srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < size; i++) {
        data[i] = std::rand() % max_value;
    }
}

// Сравнение двух массивов
extern "C" bool compareArrays(int* a, int* b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}