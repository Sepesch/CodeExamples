#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>
#include <unordered_set>
#include <iomanip>

using namespace std;
bool d = 1;
const double INF = numeric_limits<double>::infinity();
vector<vector<double>> graph;
int n;
int start;
vector<pair<vector<int>, double>> allPaths;

struct Node {
    vector<int> path;
    double bound;
    double priority;

    bool operator<(const Node& other) const {
        return priority > other.priority;
    }
};

double calculate_path_cost(const vector<int>& path) {
    double cost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        cost += graph[path[i]][path[i+1]];
    }
    return cost;
}

double calculate_bound(const vector<int>& path, 
                      const vector<pair<double, double>>& min_edges) {
    double bound = 0;
    
    // 1. Добавляем вес уже пройденного пути
    bound += calculate_path_cost(path);
    
    // 2. Добавляем полусумму двух минимальных ребер для оставшихся вершин
    unordered_set<int> visited(path.begin(), path.end());
    for (int node = 0; node < n; ++node) {
        if (visited.find(node) == visited.end()) {
            if (min_edges[node].second == INF) {
                if (min_edges[node].first == INF) return INF;
                bound += min_edges[node].first;
            } else {
                bound += (min_edges[node].first + min_edges[node].second) / 2;
            }
        }
    }
    
    return bound;
}

pair<vector<int>, double> solve_tsp() {
    if (n == 0) return {{}, 0};
   
    // Вычисляем два минимальных ребра для каждой вершины
    vector<pair<double, double>> min_edges(n);
    for (int i = 0; i < n; ++i) {
        vector<double> edges;
        for (int j = 0; j < n; ++j) {
            if (i != j) edges.push_back(graph[i][j]);
        }
        sort(edges.begin(), edges.end());
        min_edges[i] = (edges.size() >= 2) ? 
                       make_pair(edges[0], edges[1]) :
                       make_pair(edges[0], INF);
        
        // Вывод минимальных ребер для каждой вершины
        cout << "Минимальные ребра для вершины " << i << ": ";
        cout << (min_edges[i].first == INF ? "INF" : to_string(min_edges[i].first)) << ", ";
        cout << (min_edges[i].second == INF ? "INF" : to_string(min_edges[i].second)) << endl;
    }

    priority_queue<Node> pq;
    
    Node initial;
    initial.path = {0};
    initial.bound = calculate_bound(initial.path, min_edges);
    initial.priority = (1 + 0) / (0.5 * n + 1);
    pq.push(initial);

    // Вывод начального узла
    cout << "\nНачальный узел:" << endl;
    cout << "  Путь: ";
    for (int node : initial.path) cout << node << " ";
    cout << "\n  Нижняя оценка: " << initial.bound;
    cout << "\n  Приоритет: " << initial.priority << endl;

    vector<int> best_path;
    double best_cost = INF;
    
    if(n==2){
        if(graph[0][1]!=INF && graph[1][0]!=INF){
            best_cost=graph[0][1]+graph[1][0];
            best_path={0,1,0};
            allPaths.push_back({best_path, best_cost});
            
            // Вывод для случая n=2
            cout << "\nСпециальный случай для 2 вершин:" << endl;
            cout << "  Найден путь: ";
            for (int node : best_path) cout << node << " ";
            cout << "\n  Стоимость: " << best_cost << endl;
        }
    }
    else{
    int iteration = 1;
    while (!pq.empty()) {
        cout << "\n--- Итерация " << iteration++ << " ---" << endl;
        
        Node current = pq.top();
        pq.pop();

        // Вывод текущего узла
        cout << "Текущий узел:" << endl;
        cout << "  Путь: ";
        for (int node : current.path) cout << node << " ";
        cout << "\n  Нижняя оценка: " << current.bound;
        cout << "\n  Приоритет: " << current.priority << endl;
        cout << "  Лучшая известная стоимость: " << (best_cost == INF ? "INF" : to_string(best_cost)) << endl;

        if (current.bound >= best_cost) {
            cout << "  Отсечение - оценка хуже лучшего решения" << endl;
            continue;
        }

        if (current.path.size() == static_cast<size_t>(n)) {
            double cost = calculate_path_cost(current.path) + graph[current.path.back()][current.path[0]];
            cout << "  Полный путь найден. Стоимость: " << cost << endl;
            
            if (cost < best_cost) {
                best_cost = cost;
                allPaths.clear();
                best_path = current.path;
                best_path.push_back(best_path[0]);
                allPaths.push_back({best_path, best_cost});
                
                cout << "  Новый лучший путь! Стоимость: " << best_cost << endl;
                cout << "  Путь: ";
                for (int node : best_path) cout << node << " ";
                cout << endl;
            }
            current.path.push_back(current.path[0]);
            allPaths.push_back({current.path, cost});
            continue;
        }

        int last = current.path.back();
        cout << "  Генерация потомков для вершины " << last << ":" << endl;
        
        // Собираем все возможные следующие вершины
        vector<pair<double, int>> candidates;
        for (int next_node = 0; next_node < n; ++next_node) {
            if (find(current.path.begin(), current.path.end(), next_node) == current.path.end() && 
                graph[last][next_node] != INF) {
                candidates.emplace_back(graph[last][next_node], next_node);
            }
        }
        
        // Сортируем кандидатов по стоимости ребра
        sort(candidates.begin(), candidates.end());
        
        for (auto [cost, next_node] : candidates) {
            Node child;
            child.path = current.path;
            child.path.push_back(next_node);
            child.bound = calculate_bound(child.path, min_edges);
            
            cout << "  Потомок с вершиной " << next_node << ":" << endl;
            cout << "    Ребро " << last << "->" << next_node << " стоимость " << cost << endl;
            cout << "    Новый путь: ";
            for (int node : child.path) cout << node << " ";
            cout << "\n    Новая оценка: " << child.bound << endl;
            
            if (child.bound < best_cost) {
                double S = child.path.size();
                double L = calculate_path_cost(child.path);
                child.priority = (S + L) / (0.5 * n + S);
                pq.push(child);
                
                cout << "    Узел добавлен в очередь с приоритетом " << child.priority << endl;
            } else {
                cout << "    Отсечение - оценка " << child.bound << " >= лучшей стоимости " << best_cost << endl;
            }
        }
    }    
    }    

    return {best_path, best_cost};
}

vector<int> nearest_neighbor_tsp() {
    if (n == 0) return {};

    vector<int> tour;
    vector<bool> visited(n, false);
    
    int current_city = start;
    tour.push_back(current_city);
    visited[current_city] = true;

    for (int i = 1; i < n; ++i) {
        double min_dist = INF;
        int nearest_city = -1;
        double min3 = INF;
        int The_city = -1;
        
        cout << "\nТекущий город: " << current_city << endl;
        cout << "Поиск ближайшего соседа:" << endl;
        
        for (int city = 0; city < n; ++city) {
            if (!visited[city] && graph[current_city][city] < min_dist) {
                min_dist = graph[current_city][city];
                nearest_city = city;
                int nearest_toThat = -1;
                cout << "  Найден ближайший город " << city << " (расстояние " << min_dist << ")" << endl;
                
                double min2 = INF;
                for(int c2 = 0; c2<n; ++c2){
                    if(!visited[c2] && graph[nearest_city][c2] < min2){
                        min2 = graph[nearest_city][c2];
                        nearest_toThat = c2;
                    }
                }
                
                double heuristic = min_dist + min2 - graph[current_city][nearest_toThat];
                cout << "  Эвристика: " << min_dist << " + " << min2 << " - " 
                     << graph[current_city][nearest_toThat] << " = " << heuristic << endl;
                
                if(!visited[city] && heuristic < min3){
                    min3 = heuristic;
                    The_city = nearest_city;
                    cout << "  Новый лучший кандидат: " << The_city << " (оценка " << min3 << ")" << endl;
                }
            }
        }

        if (The_city != -1) {
            tour.push_back(The_city);
            visited[The_city] = true;
            current_city = The_city;
            cout << "Переход в город " << The_city << endl;
        }
    }

    tour.push_back(tour[0]);
    cout << "Завершение цикла, возврат в начальный город " << tour[0] << endl;
    return tour;
}

int main() {
    bool choice = 0;
    cin >> start;
    cin.ignore();
    string l;
    
    while(getline(cin,l)){
        if(l.empty()) break;
        vector<double> r;
        size_t p = 0;
        while(p < l.length()){
            double n = stod(l.substr(p));
            r.push_back(n == -1 ? INF : n);
            p = l.find(' ', p);
            if(p == string::npos) break;
            p++;
        }
        graph.push_back(r);
    }
    n = graph.size();
    start = 0;
    
    if(choice){
        cout << "=== Начало работы алгоритма ветвей и границ ===" << endl;
        auto res = solve_tsp();
        cout << "\n=== Результат ===" << endl;
        for(int a = 0; a < res.first.size()-1; a++){
            if(a < res.first.size()-2) cout << res.first[a] << " ";
            else cout << res.first[a];
        }
        cout << endl << setprecision(1) << fixed << res.second;
    }
    else{
        cout << "=== Начало работы алгоритма ближайшего соседа ===" << endl;
        for (int i = 0; i < graph.size(); ++i) {
            for (int j = 0; j < graph.size(); ++j) {
                if (graph[i][j] == -1) graph[i][j] = INF;
            }
        }
        
        vector<int> tour = nearest_neighbor_tsp();
        cout << "\n=== Результат ===" << endl;
        cout << calculate_path_cost(tour) << endl;
        for (int city : tour) {
            cout << city << " ";
        }
    }
    
    return 0;
}