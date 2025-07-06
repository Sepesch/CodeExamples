#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <set>
#include <fstream>
#include <cstdlib> // для system()
using namespace std;

struct Node {
    map<char, Node*> children;
    Node* fail = nullptr;
    Node* dict = nullptr;
    vector<pair<int, int>> pattern_info; // pattern index and offset
    bool end = false;
    vector<int> output;
    string word;                  // Слово, заканчивающееся в этом узле (если есть)
    string id;                   // Уникальный идентификатор узла
    int patIndex = -1;        // Индекс шаблона (если узел является концом шаблона)

    Node(string id) : id(id) {}

    string toString() {
        ostringstream oss;
        oss << "Node{id='" << id << "', word='";
        if (!word.empty()) oss << word;
        oss << "', patternIndex=" << patIndex << "}";
        return oss.str();
    }

};

class AhoCorasick {
    Node* root;
    vector<pair<string, int>> subpatterns; // subpattern and its offset
    int pattern_len;
    vector<string> patterns;
    //дерево для стандартной работы алгоритма
    void buildTrie() {
        root = new Node("root");
        for (int i = 0; i < patterns.size(); ++i) {
            Node* cur = root;
            string way;
            for (char c : patterns[i]) {
                way+=c;
                if (!cur->children[c]) {
                    cur->children[c] = new Node(way);
                }
                cur = cur->children[c];
            }
            cur->output.push_back(i);
        }
    }
    //дерево для шаблона с джокером
    void SpecbuildTrie() {
        
        root = new Node("root");
        for (int i = 0; i < subpatterns.size(); ++i) {
            Node* cur = root;
            string way;
            for (char c : subpatterns[i].first) {
                way+=c;
                if (!cur->children[c]) {
                    cur->children[c] = new Node(way);
                }
                cur = cur->children[c];
            }
            cur->end = true;
            cur->pattern_info.emplace_back(i, subpatterns[i].second);
        }
    }
    //создание суффиксных ссылок
    void buildFailureLinks() {
        queue<Node*> q;
        root->fail = root;
        
        for (auto& pair : root->children) {
            pair.second->fail = root;
            q.push(pair.second);
        }

        while (!q.empty()) {
            Node* cur = q.front();
            q.pop();

            for (auto& pair : cur->children) {
                Node* child = pair.second;
                Node* failNode = cur->fail;
                
                while (failNode != root && !failNode->children[pair.first]) {
                    failNode = failNode->fail;
                }
                
                child->fail = failNode->children[pair.first] ? failNode->children[pair.first] : root;
                child->output.insert(child->output.end(), child->fail->output.begin(), child->fail->output.end());
                q.push(child);
            }
        }
    }

public:
    //построение дерева для обычного случая
    void Aho(vector<string> pats){
        patterns = pats;
        buildTrie();
        buildFailureLinks();
    }
    //построение дерева для шаблона с джокером
    void SpecAho(const vector<pair<string, int>>& subpats, int len){
        subpatterns = subpats;
        pattern_len = len;
        SpecbuildTrie();
        buildFailureLinks();
    }
    //поиск подстрок в строке
    vector<pair<int, int>> search(string text) {
        vector<pair<int, int>> results;
        Node* cur = root;
        
        for (int i = 0; i < text.size(); ++i) {
            char c = text[i];
            while (cur != root && !cur->children[c]) {
                cur = cur->fail;
            }
            
            cur = cur->children[c] ? cur->children[c] : root;
            
            for (int patternIdx : cur->output) {
                results.emplace_back(i - patterns[patternIdx].size() + 2, patternIdx + 1);
            }
        }
        return results;
    }
    //поиск вхождений подстроки с джокером
    vector<int> SpecSearch(const string& text) {
        vector<int> pattern_pos(text.size() + 1, 0);
        Node* cur = root;

        for (int i = 0; i < text.size(); ++i) {
            while (cur != root && !cur->children[text[i]]) {
                cur = cur->fail;
            }
            cur = cur->children[text[i]] ? cur->children[text[i]] : root;

            for (Node* state = cur; state != root; state = state->fail) {
                if (state->end) {
                    for (auto& [pattern_idx, offset] : state->pattern_info) {
                        int index = i - subpatterns[pattern_idx].first.size() + 1 - offset;
                        if (index >= 0 && index + pattern_len <= text.size()) {
                            pattern_pos[index]++;
                        }
                    }
                }
            }
        }

        vector<int> result;
        for (int i = 0; i <= text.size() - pattern_len; ++i) {
            if (pattern_pos[i] == subpatterns.size()) {
                result.push_back(i + 1);
            }
        }
        return result;
    }
    //визуализация работы программы для стандартного случая
    void visualize() {
        cout << "=== Визуализация автомата Ахо-Корасик ===" << endl;
        queue<Node*> q;
        set<Node*> visited;
        q.push(root);
        visited.insert(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            
            cout << "Узел " << current->id << ":";
            if (!current->word.empty()) cout << " (слово: " << current->word << ")";
            cout << endl;

            // Переходы
            if(!current->children.empty()){
                for (auto& entry : current->children) {
                    if(entry.second){
                    cout << "  --" << entry.first << "--> " << entry.second->id << endl;
                if (visited.find(entry.second) == visited.end()) {
                    visited.insert(entry.second);
                    q.push(entry.second);
                }
                    }

            }
        }
            if (current->fail!= nullptr && current->fail != root) {
                cout << "  --failure--> " << current->fail->id << endl;
            }
        
    }
        cout << "=== Конец визуализации ===" << endl;
    }
    //визуализация работы программы для случая с джокером
    void SpecVisualize(const string& pattern, char spec) {
        cout << "=== Визуализация поиска с джокером ===" << endl;
        cout << "Шаблон: " << pattern << endl;
        cout << "Джокер: '" <<spec << "'" << endl;
        
        cout << "Граф переходов:" << endl;
        cout << "[start]";
        for (char c : pattern) {
            cout << " --" << (c == spec ? "любой символ" : string(1, c)) << "--> ";
        }
        cout << "[end]" << endl;
        cout << "=== Конец визуализации ===" << endl;
    }
    //генерация dot файла дерева
        void generateDotFile(const string& filename) {
        ofstream dotFile(filename);
        dotFile << "digraph AhoCorasick {" << endl;
        dotFile << "    rankdir=LR;" << endl;
        dotFile << "    node [shape=circle];" << endl;
        
        queue<Node*> q;
        set<Node*> visited;
        q.push(root);
        visited.insert(root);

        // Сначала добавляем все узлы
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            
            // Определяем стиль узла
            string nodeStyle = "regular";
            if (current == root) {
                nodeStyle = "bold";
            }
            if (!current->output.empty()) {
                nodeStyle = "filled";
            }
            
            // Добавляем узел
            dotFile << "    \"" << current->id << "\" [style=" << nodeStyle;
            if (!current->output.empty()) {
                dotFile << ", fillcolor=lightgray";
            }
            dotFile << "];" << endl;
            
            // Добавляем переходы
            for (auto& entry : current->children) {
                if (entry.second) {
                    dotFile << "    \"" << current->id << "\" -> \"" << entry.second->id 
                             << "\" [label=\"" << entry.first << "\"];" << endl;
                    if (visited.find(entry.second) == visited.end()) {
                        visited.insert(entry.second);
                        q.push(entry.second);
                    }
                }
            }
            
            // Добавляем fail-ссылки
            if (current->fail && current->fail != root) {
                dotFile << "    \"" << current->id << "\" -> \"" << current->fail->id 
                         << "\" [style=dotted, color=red];" << endl;
            }
        }
        
        dotFile << "}" << endl;
        dotFile.close();
        cout << "Граф автомата сохранен в файл " << filename << endl;
        cout << "Для визуализации выполните команду: dot -Tpng " << filename << " -o automaton.png" << endl;
        
    }
    
//генерация изображения построенного дерева шаблонов
void generateAndShowGraph() {
    const std::string dotFile = "automaton.dot";
    const std::string pngFile = "automaton.png";
    
    // 1. Генерируем DOT-файл
    generateDotFile(dotFile);
    
    // 2. Конвертируем в PNG
    std::string command = "dot -Tpng " + dotFile + " -o " + pngFile;
    if (system(command.c_str())) {
        std::cerr << "Ошибка: Graphviz не установлен или команда dot не работает." << std::endl;
        return;
    }
    
    // 3. Открываем PNG (кросс-платформенно)
    #ifdef _WIN32
        command = "start " + pngFile;
    #elif __APPLE__
        command = "open " + pngFile;
    #else // Linux
        command = "xdg-open " + pngFile;
    #endif
    
    system(command.c_str());
}   
};
//функция для разбиени строки с джокером на подстроки
vector<pair<string, int>> split_pattern(const string& pattern, char wild) {
    vector<pair<string, int>> subpatterns;
    string current;
    int start_pos = 0;

    for (size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != wild) {
            if (current.empty()) {
                start_pos = i;
            }
            current += pattern[i];
        }
        else {
            if (!current.empty()) {
                subpatterns.emplace_back(current, start_pos);
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        subpatterns.emplace_back(current, start_pos);
    }
    return subpatterns;
}

int main() {

    string text, pattern;
    getline(cin, text);          // Чтение текста
    getline(cin, pattern);    // Чтение второй строки

    bool isVisualize = true;     // Флаг для включения/отключения визуализации

    try {
        int n = stoi(pattern); // Попытка преобразовать вторую строку в число
        vector<string> patterns(n);
        for (int i = 0; i < n; i++) {
            getline(cin, patterns[i]); // Чтение шаблонов
        }

        AhoCorasick ac;
        ac.Aho(patterns);
        if (isVisualize) {
            ac.visualize();
            ac.generateAndShowGraph();       // Визуализация автомата
        }
        vector<pair<int, int>> result = ac.search(text);
        sort(result.begin(), result.end());
        for (auto& pair : result) { // Поиск и вывод результатов
            cout << pair.first<<" "<<pair.second << endl;
        }
    } catch (const invalid_argument&) {
        // Если вторая строка не является числом, интерпретируем ее как шаблон с джокером
        char spec;
        string specStr;
        getline(cin, specStr);
        spec = specStr[0]; // Чтение символа джокера
        
        auto subpatterns = split_pattern(pattern, spec);
        if (subpatterns.empty()) {
            cout << -1 << endl;
            return 0;
        }

        AhoCorasick ac;
        ac.SpecAho(subpatterns, pattern.size());

        if (isVisualize) {
            ac.SpecVisualize(pattern, spec);
            ac.generateAndShowGraph();   // Визуализация поиска с джокером
        }
        for (auto& pair : ac.SpecSearch(text)) { // Поиск и вывод результатов
            cout << pair<< endl;
        }
        }
        

    return 0;
}