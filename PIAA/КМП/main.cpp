
#include <iostream>
#include <vector>
#include <string>

using namespace std;
//Функция, высчитывающая префикс-функцию подстроки
vector<int> computePrefixFunction(const string& pattern) {
    cout<<"Вычисление префикс-функции для подстроки: "<< pattern<<endl;
    int m = pattern.size();
    vector<int> pi(m, 0);//вектор для хранения значение префикс-функции
    for (int i = 1, j = 0; i < m; ++i) {
        cout<<"Шаг "<<i<<" (символ '"<<pattern[i]<<"')"<<endl;
        cout<<"Начальный j = "<<j<<endl;
        while (j > 0 && pattern[i] != pattern[j]) {
            cout << "- Несовпадение: pattern[" << i << "] != pattern[" << j << "] → j = pi[" << j-1 << "] = " << pi[j-1] << endl;
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            cout << "- Совпадение: pattern[" << i << "] == pattern[" << j << "] → j++" << endl;
            j++;
        }
        pi[i] = j;
    }
    return pi;
}
//функция нахождения вхождения подстроки в строки методом КМП
vector<int> KMPSearch(const string& text, const string& pattern) {
    vector<int> result;
    int n = text.size();
    int m = pattern.size();
    
    if (m == 0 || n < m) {//если длина шаблона 0 или шаблон длинее такста, возвращается пустой вектор
        return result;
    }
    cout<<endl;
    vector<int> pi = computePrefixFunction(pattern);//значения префикс-функции для подстроки
    cout<<"\n Итоговый массив префикс-функции:[ ";
    for(int i=0; i<pi.size(); i++){cout<<pi[i]<<" ";}cout<<"]"<<endl;
    cout << "\nНачало поиска.\n";
    for (int i = 0, j = 0; i < n; ++i) {
        cout << "Шаг " << i << ": text[" << i << "] = '" << (i < n ? text[i] : ' ') << "', pattern[" << j << "] = '" << (j < m ? pattern[j] : ' ') << "'\n";
        while (j > 0 && text[i] != pattern[j]) {
            j = pi[j - 1];
        }
        if (text[i] == pattern[j]) {
            cout << "  Совпадение! Увеличиваем j\n";
            j++;
        }
        if (j == m) {
            cout << "  Найдено полное совпадение на позиции " << i - j << "!\n";
            result.push_back(i - m + 1);
            j = pi[j - 1];
            cout << "  Сдвигаем j на " << j << "\n";
        }
        cout << "  Текущие индексы: i = " << i << ", j = " << j << "\n";
        cout << "----------------------------------------\n";
    }
    return result;
}

int main() {
    bool opt = false; //переменная опции, для определения рабоыт программы
    if (opt){
        string P, T;//поиск вхождений подстроки в строку
        //Ввод подстроки и строки
        getline(cin, P);
        getline(cin, T);
        //запуск поиска
        vector<int> occurrences = KMPSearch(T, P);
        //вывод результата
        cout<<"\n Индексы вхождния подстроки в строку: ";
        if (occurrences.empty()) {
            cout << -1 << endl;
        } else {
            for (size_t i = 0; i < occurrences.size(); ++i) {
                if (i != 0) {
                    cout << " ";
                }
                cout << occurrences[i];
            }
            cout << endl;
        }
    }
    else{
    string P, T;//поиск смещения между двумя равными строками
    //Ввод строки и подстроки
    getline(cin, T);
    getline(cin, P);
    if(T.length() != P.length()){//если их длины не равны, программа завершается
        cout<<-1;
        return 0;
    }
    T.append(T);
    //запуск поиска
    vector<int> occurrences = KMPSearch(T, P);
    //вывод результата
    cout<<"Смещение: ";
    if (occurrences.empty()) {
        cout << -1 << endl;
    } else {
        cout<<occurrences[0];
    }
    }


    return 0;
}