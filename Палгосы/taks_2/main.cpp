#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cmath>

template<typename T>
class ListNode {
public:
    T data;
    std::shared_ptr<ListNode<T>> next;
    
    ListNode(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class ThreadSafeList {
public:
    virtual bool insert(const T& value) = 0;
    virtual bool remove(const T& value) = 0;
    virtual bool find(const T& value) = 0;
    virtual size_t size() const = 0;
    virtual ~ThreadSafeList() = default;
};

template<typename T>
class CoarseGrainedList : public ThreadSafeList<T> {
private:
    std::shared_ptr<ListNode<T>> head;
    mutable std::mutex mtx;
    std::atomic<size_t> list_size{0};

public:
    CoarseGrainedList() : head(nullptr) {}

    bool insert(const T& value) override {
        std::lock_guard<std::mutex> lock(mtx);
        
        auto newNode = std::make_shared<ListNode<T>>(value);
        
        if (!head || value < head->data) {
            newNode->next = head;
            head = newNode;
            list_size.fetch_add(1);
            return true;
        }
        
        auto current = head;
        while (current->next && current->next->data < value) {
            current = current->next;
        }
        
        if (current->next && current->next->data == value) {
            return false;
        }
        
        newNode->next = current->next;
        current->next = newNode;
        list_size.fetch_add(1);
        return true;
    }

    bool remove(const T& value) override {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (!head) return false;
        
        if (head->data == value) {
            head = head->next;
            list_size.fetch_sub(1);
            return true;
        }
        
        auto current = head;
        while (current->next && current->next->data < value) {
            current = current->next;
        }
        
        if (current->next && current->next->data == value) {
            current->next = current->next->next;
            list_size.fetch_sub(1);
            return true;
        }
        
        return false;
    }

    bool find(const T& value) override {
        std::lock_guard<std::mutex> lock(mtx);
        
        auto current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            if (current->data > value) {
                break;
            }
            current = current->next;
        }
        return false;
    }

    size_t size() const override {
        return list_size.load();
    }
};

template<typename T>
class FineGrainedList : public ThreadSafeList<T> {
private:
    struct Node {
        T data;
        std::shared_ptr<Node> next;
        std::mutex mtx;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    std::shared_ptr<Node> head;
    std::atomic<size_t> list_size{0};

public:
    FineGrainedList() : head(std::make_shared<Node>(T{})) {}

    bool insert(const T& value) override {
        auto pred = head;
        pred->mtx.lock();
        
        try {
            auto curr = pred->next;
            if (curr) {
                curr->mtx.lock();
            }
            
            while (curr && curr->data < value) {
                pred->mtx.unlock();
                pred = curr;
                curr = curr->next;
                if (curr) {
                    curr->mtx.lock();
                }
            }
            
            if (curr && curr->data == value) {
                if (curr) curr->mtx.unlock();
                pred->mtx.unlock();
                return false;
            }
            
            auto newNode = std::make_shared<Node>(value);
            newNode->next = curr;
            pred->next = newNode;
            list_size.fetch_add(1);
            
            if (curr) curr->mtx.unlock();
            pred->mtx.unlock();
            return true;
            
        } catch (...) {
            if (pred) pred->mtx.unlock();
            throw;
        }
    }

    bool remove(const T& value) override {
        auto pred = head;
        pred->mtx.lock();
        
        try {
            auto curr = pred->next;
            if (curr) {
                curr->mtx.lock();
            }
            
            while (curr && curr->data < value) {
                pred->mtx.unlock();  
                pred = curr;
                curr = curr->next;
                if (curr) {
                    curr->mtx.lock();
                }
            }
            
            if (curr && curr->data == value) {
                pred->next = curr->next;
                list_size.fetch_sub(1);
                curr->mtx.unlock();
                pred->mtx.unlock();
                return true;
            }
            
            if (curr) curr->mtx.unlock();
            pred->mtx.unlock();
            return false;
            
        } catch (...) {
            if (pred) pred->mtx.unlock();
            throw;
        }
    }

    bool find(const T& value) override {
        auto curr = head->next;
        std::shared_ptr<Node> prev;
        
        while (curr) {
            std::unique_lock<std::mutex> lock(curr->mtx);
            if (curr->data == value) {
                return true;
            }
            if (curr->data > value) {
                return false;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    size_t size() const override {
        return list_size.load();
    }
};

template<typename T>
class LockFreeList : public ThreadSafeList<T> {
private:
    struct Node {
        T data;
        std::atomic<Node*> next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    std::atomic<Node*> head;
    std::atomic<size_t> list_size{0};

    static bool is_marked(Node* node) {
        return reinterpret_cast<uintptr_t>(node) & 1;
    }
    
    static Node* get_unmarked(Node* node) {
        return reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(node) & ~1);
    }
    
    static Node* get_marked(Node* node) {
        return reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(node) | 1);
    }

public:
    LockFreeList() : head(nullptr) {}

    bool insert(const T& value) override {
        Node* newNode = new Node(value);
        Node* oldHead = head.load(std::memory_order_acquire);
        newNode->next.store(oldHead, std::memory_order_release);
        
        while (!head.compare_exchange_weak(oldHead, newNode,
                                         std::memory_order_release,
                                         std::memory_order_acquire)) {
            newNode->next.store(oldHead, std::memory_order_release);
        }
        
        list_size.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    bool remove(const T& value) override {
        while (true) {
            Node* pred = head.load(std::memory_order_acquire);
            Node* curr = pred;
            
            while (curr) {
                Node* succ = curr->next.load(std::memory_order_acquire);
                
                if (is_marked(succ)) {
                    break;
                }
                
                if (curr != head && curr->data == value) {
                    Node* expected = succ;
                    Node* desired = get_marked(succ);
                    
                    if (curr->next.compare_exchange_weak(expected, desired,
                                                       std::memory_order_release,
                                                       std::memory_order_acquire)) {
                        if (pred->next.compare_exchange_weak(curr, succ,
                                                           std::memory_order_release,
                                                           std::memory_order_acquire)) {
                            list_size.fetch_sub(1, std::memory_order_relaxed);
                        }
                        return true;
                    }
                }
                
                pred = curr;
                curr = get_unmarked(succ);
            }
            
            return false;
        }
    }

    bool find(const T& value) override {
        Node* curr = head.load(std::memory_order_acquire);
        
        while (curr != nullptr) {
            Node* succ = curr->next.load(std::memory_order_acquire);
            
            if (is_marked(succ)) {
                curr = get_unmarked(succ);
                continue;
            }
            
            if (curr->data == value) {
                return true;
            }
            curr = get_unmarked(succ);
        }
        return false;
    }

    size_t size() const override {
        return list_size.load(std::memory_order_relaxed);
    }

    ~LockFreeList() {
        Node* current = head.load(std::memory_order_acquire);
        while (current != nullptr) {
            Node* next = get_unmarked(current->next.load(std::memory_order_acquire));
            delete current;
            current = next;
        }
    }
};

class TestPipeline {
private:
    std::ofstream results_file;

    template<typename ListType>
    struct TestResult {
        double throughput_ops_per_sec;
        double avg_latency_ms;
        size_t successful_operations;
        size_t total_operations;
        size_t read_operations;
        size_t write_operations;
    };

    template<typename ListType>
    TestResult<ListType> run_throughput_test(ListType& list, 
                                           int total_threads,
                                           double read_ratio,
                                           int duration_seconds,
                                           int initial_size) {
        for (int i = 0; i < initial_size; ++i) {
            list.insert(i);
        }

        std::atomic<bool> stop{false};
        std::atomic<size_t> total_operations{0};
        std::atomic<size_t> successful_operations{0};
        std::atomic<size_t> read_ops{0};
        std::atomic<size_t> write_ops{0};
        std::vector<std::thread> threads;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        int reader_threads = static_cast<int>(total_threads * read_ratio);
        int writer_threads = total_threads - reader_threads;
        
        for (int i = 0; i < reader_threads; ++i) {
            threads.emplace_back([&, i]() {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, initial_size * 2);
                
                while (!stop.load()) {
                    int value = dis(gen);
                    bool found = list.find(value);
                    total_operations.fetch_add(1, std::memory_order_relaxed);
                    read_ops.fetch_add(1, std::memory_order_relaxed);
                    if (found) successful_operations.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        
        for (int i = 0; i < writer_threads; ++i) {
            threads.emplace_back([&, i]() {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, initial_size * 2);
                std::uniform_real_distribution<> op_dis(0.0, 1.0);
                
                while (!stop.load()) {
                    int value = dis(gen);
                    if (op_dis(gen) < 0.5) {
                        if (list.insert(value)) {
                            successful_operations.fetch_add(1, std::memory_order_relaxed);
                        }
                    } else {
                        if (list.remove(value)) {
                            successful_operations.fetch_add(1, std::memory_order_relaxed);
                        }
                    }
                    total_operations.fetch_add(1, std::memory_order_relaxed);
                    write_ops.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(duration_seconds));
        stop.store(true, std::memory_order_release);
        
        for (auto& t : threads) {
            t.join();
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time).count();
        
        double throughput = (total_operations.load() * 1000.0) / duration_ms;
        double avg_latency = (duration_ms * 1000.0) / total_operations.load();
        
        return TestResult<ListType>{throughput, avg_latency, 
                                  successful_operations.load(), 
                                  total_operations.load(),
                                  read_ops.load(),
                                  write_ops.load()};
    }

public:
    TestPipeline() {
    }

    ~TestPipeline() {
    }

    template<typename ListType>
    void run_read_ratio_test(const std::string& list_name) {
        
        std::cout << "\n=== Тестирование " << list_name << " (Пропускная способность vs доля читателей) ===" << std::endl;
        
        std::vector<int> thread_counts = {1, 2, 4, 8, 12, 16, 20, 24};
        std::vector<double> read_ratios = {0.0, 0.25, 0.5, 0.75, 1.0};
        const int DURATION_SECONDS = 1;  
        const int INITIAL_SIZE = 1000;
        
        std::cout << "Threads\\ReadRatio";
        for (double ratio : read_ratios) {
            std::cout << " | " << std::fixed << std::setprecision(0) << (ratio * 100) << "%";
        }
        std::cout << std::endl;
        
        for (int threads : thread_counts) {
            std::cout << std::setw(3) << threads;
            
            for (double read_ratio : read_ratios) {
                ListType list;
                auto result = run_throughput_test<ListType>(list, threads, read_ratio, 
                                                          DURATION_SECONDS, INITIAL_SIZE);
                
                std::cout << " | " << std::fixed << std::setprecision(0) 
                         << std::setw(8) << result.throughput_ops_per_sec;
                
            }
            
            std::cout << std::endl;
        }
        
    }
};

int main() {
    TestPipeline pipeline;
    
    pipeline.run_read_ratio_test<CoarseGrainedList<int>>("CoarseGrainedList");
    pipeline.run_read_ratio_test<FineGrainedList<int>>("FineGrainedList"); 
    pipeline.run_read_ratio_test<LockFreeList<int>>("LockFreeList");
    
    return 0;
}