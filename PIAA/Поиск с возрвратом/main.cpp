#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> result; // Лучшее решение
vector<vector<int>> current; // Текущее решение
vector<vector<int>> Best;

void drawMap(const vector<vector<int>>& map){
    for (int i=0; i<map.size(); i++){
        for(int j=0; j<map[i].size(); j++){
            cout<<map[i][j]<<" ";
        }
        cout<<"\n";
    }
}
int isComp(int number)
{
    for(int i=2; i<number; i++){if(number%i==0)return i;}
    return 0;
}
// Проверка, можно ли разместить квадрат размера size в позиции (x, y)
bool IsValid(int x, int y, int size, int n, const vector<vector<int>>& map) {
    if (x + size > n || y + size > n) return false;
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            if (map[i][j] != 0) return false;
        }
    }
    return true;
}

// Размещение квадрата размера size в позиции (x, y)
void PlaceSquare(int x, int y, int size, vector<vector<int>>& map) {
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            map[i][j] = current.size() + 1;
        }
    }
    current.push_back({x, y, size});
}

// Удаление квадрата размера size из позиции (x, y)
void RemoveSquare(int x, int y, int size, vector<vector<int>>& map) {
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            map[i][j] = 0;
        }
    }
    current.pop_back();
}

// Рекурсивная функция для поиска минимального разбиения
void Req(int ind, int n, vector<vector<int>>& map) {
    // Если текущее решение хуже лучшего, прекращаем поиск
    if (!result.empty() && (current.size() >= result.size())) return;
    
    if(current.empty()){
        int tmp = isComp(n);
        if(!tmp){
            PlaceSquare(0,0,(n+1)/2, map);
            PlaceSquare((n+1)/2,0,n/2, map);
            PlaceSquare(0,(n+1)/2,n/2, map);
        }
        else{
            PlaceSquare(0,0,(n/tmp)*((tmp+1)/2), map);
            PlaceSquare((n/tmp)*((tmp+1)/2),0,(n/tmp)*(tmp/2), map);
            PlaceSquare(0,(n/tmp)*((tmp+1)/2),(n/tmp)*(tmp/2), map);
        }
    }
    // Находим первую пустую клетку
    while (ind < n * n) {
        if (map[ind / n][ind % n] == 0) break;
        ind++;
    }

    // Если все клетки заполнены, сохраняем решение
    if (ind == n * n) {
        if (result.empty() || current.size() < result.size()) {
            Best = map;
            result = current;
        }
        return;
    }
    // Пробуем квадраты от максимального до минимального размера
    int maxSize = min(min(n-1, n-ind/n), n-ind%n);
    for (int size = maxSize; size >=1; --size) {
        // Проверяем, можно ли разместить квадрат
        if (IsValid(ind / n, ind % n, size, n, map)) {
            // Размещаем квадрат
            PlaceSquare(ind / n, ind % n, size, map);
            drawMap(map); cout<<"\n";
            // Рекурсивный вызов
            Req(ind + size, n, map);
            
            // Откат изменений
            RemoveSquare(ind / n, ind % n, size, map);
        }
    }
}

int main() {
    int n;
    cin>>n;

    // Инициализация сетки
    vector<vector<int>> map(n, vector<int>(n, 0));
    // Запуск рекурсивного поиска
    const clock_t c_start = clock();
    Req(0,n, map);
    const clock_t c_end = clock();
    //cout<<(c_end-c_start)<<" ms."<<"\n";
    // Вывод результата
    
    cout << result.size() << "\n";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i][0] + 1 << " " << result[i][1] + 1 << " " << result[i][2] << "\n";
    }
    cout<<"\n";
    drawMap(Best);
    return 0;
}