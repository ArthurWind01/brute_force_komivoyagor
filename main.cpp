#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_set>

struct pair_hash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); // Combine the hash values
    }
};

struct Graph {
public:
    int n;
    std::unordered_map<std::pair<int, int>, int, pair_hash> adjList;
    std::vector<std::pair<int, int>> adjList1;

    void addEdge(const int source, const int destination, int weight) {

        adjList[{source, destination}] = weight;
        adjList1.push_back({source,destination});
    }

    int getEdgeVal(const int source, const int destination) {

        return adjList[{source, destination}];
    }

    void brute_force_razam() {
        int m=adjList1.size();
        int min = INT_MAX;
        std::vector<std::pair<int, int>> adjCounter;

        std::vector<bool> counter(m, false);


        for (int i = 0; i < (1 << m); ++i) {
            std::vector<std::pair<int, int>> local_counter;
            int local_min=0;
            for (int j = 0; j < m; ++j) {
                if(counter[j]){
                    local_counter.push_back(adjList1[j]);
                    local_min+=adjList[{adjList1[j].first,adjList1[j].second}];
                }
            }
            if(hasHamiltonianPath(local_counter,n) && min>local_min){
                adjCounter.clear();
                min=local_min;
                adjCounter.insert(adjCounter.begin(),local_counter.begin(),local_counter.end());
            }
            for (int j = 0; j < m; ++j) {
                counter[j] = !counter[j];
                if (counter[j]) break;
            }
        }
        for (std::pair<int,int> num: adjCounter) {
            std::cout <<"{"<< num.first<<","<<num.second << "} ";
        }
        std::cout << " -> " << min;
        std::cout << std::endl;
    }

    void brute_force_zam() {
        int m=adjList1.size();
        int min = INT_MAX;
        std::vector<std::pair<int, int>> adjCounter;

        std::vector<bool> counter(m, false);


        for (int i = 0; i < (1 << m); ++i) {
            std::vector<std::pair<int, int>> local_counter;
            int local_min=0;
            for (int j = 0; j < m; ++j) {
                if(counter[j]){
                    local_counter.push_back(adjList1[j]);
                    local_min+=adjList[{adjList1[j].first,adjList1[j].second}];
                }
            }
            if(hasHamiltonianCycle(local_counter,n) && min>local_min){
                adjCounter.clear();
                min=local_min;
                adjCounter.insert(adjCounter.begin(),local_counter.begin(),local_counter.end());
            }
            for (int j = 0; j < m; ++j) {
                counter[j] = !counter[j];
                if (counter[j]) break;
            }
        }
        for (std::pair<int,int> num: adjCounter) {
            std::cout <<"{"<< num.first<<","<<num.second << "} ";
        }
        std::cout << " -> " << min;
        std::cout << std::endl;
    }

    void perest_zam() {
        std::vector<int> main_permutation;
        int min = INT_MAX;


        // Изначально создаем вектор с числами от 1 до n
        std::vector<int> permutation;
        for (int i = 2; i <= n; ++i) {
            permutation.push_back(i);
        }

        // Генерация всех перестановок
        do {
            int local_min = 0;
            bool flag = true;
            // Вывод текущей перестановки
            if (adjList[{1, permutation[0]}]) {
                local_min += adjList[{1, permutation[0]}];
            } else {
                flag = false;
                continue;
            }
            for (int i = 0; i < permutation.size()-1; i++) {
                if (adjList[{permutation[i], permutation[i + 1]}]) {
                    local_min += adjList[{permutation[i], permutation[i + 1]}];
                } else {
                    flag = false;
                    break;
                }
            }
            if (adjList[{permutation[permutation.size() - 1], 1}]) {
                local_min += adjList[{permutation[permutation.size() - 1], 1}];
            } else {
                flag = false;
                continue;
            }
            if (local_min < min && flag) {
                main_permutation.clear();
                min = local_min;
                main_permutation.push_back(1);
                main_permutation.insert(main_permutation.end(), permutation.begin(), permutation.end());
                main_permutation.push_back(1);

                for (int num: main_permutation) {
                    std::cout << num << " ";
                }
                std::cout << " -> " << min;
                std::cout << std::endl;
            }
        } while (std::next_permutation(permutation.begin(), permutation.end()));

    }

    void perest_razam() {
        std::vector<int> main_permutation;
        int min = INT_MAX;

        // Изначально создаем вектор с числами от 1 до n
        std::vector<int> permutation;
        for (int i = 1; i <= n; ++i) {
            permutation.push_back(i);
        }

        // Генерация всех перестановок
        do {
            int local_min = 0;
            bool flag = true;
            // Вывод текущей перестановки

            for (int i = 0; i < permutation.size()-1; i++) {
                if (adjList[{permutation[i], permutation[i + 1]}]) {
                    local_min += adjList[{permutation[i], permutation[i + 1]}];
                } else {
                    flag = false;
                    break;
                }
            }
            if (local_min < min && flag) {
                main_permutation.clear();
                min = local_min;
                main_permutation.insert(main_permutation.end(), permutation.begin(), permutation.end());

                for (int num: main_permutation) {
                    std::cout << num << " ";
                }
                std::cout << " -> " << min;
                std::cout << std::endl;
            }
        } while (std::next_permutation(permutation.begin(), permutation.end()));
    }

private:
    std::unordered_map<int, std::vector<int>> buildGraph(const std::vector<std::pair<int, int>>& adjList) {
        std::unordered_map<int, std::vector<int>> graph;
        for (const auto& edge : adjList) {
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first); // Если граф неориентированный
        }
        return graph;
    }

    bool dfs(int current, int verticesCount, std::unordered_map<int, std::vector<int>>& graph, std::unordered_set<int>& visited) {
        // Если все вершины посещены, возвращаем true
        if (visited.size() == verticesCount) {
            return true;
        }

        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) { // Если вершина еще не посещена
                visited.insert(neighbor);
                if (dfs(neighbor, verticesCount, graph, visited)) {
                    return true;
                }
                visited.erase(neighbor); // Backtracking
            }
        }

        return false;
    }
    bool dfs(int current, int start, int verticesCount, std::unordered_map<int, std::vector<int>>& graph, std::unordered_set<int>& visited) {
        // Если все вершины посещены
        if (visited.size() == verticesCount) {
            // Проверяем, есть ли ребро, возвращающее нас в начальную вершину
            for (int neighbor : graph[current]) {
                if (neighbor == start) return true;
            }
            return false;
        }

        // Обходим соседей текущей вершины
        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) { // Если вершина еще не посещена
                visited.insert(neighbor);
                if (dfs(neighbor, start, verticesCount, graph, visited)) {
                    return true;
                }
                visited.erase(neighbor); // Backtracking
            }
        }

        return false;
    }
    bool hasHamiltonianPath(const std::vector<std::pair<int, int>>& adjList, int verticesCount) {
        // Строим граф
        std::unordered_map<int, std::vector<int>> graph = buildGraph(adjList);

        // Пробуем начать путь с каждой вершины
        for (int start = 0; start < verticesCount; ++start) {
            std::unordered_set<int> visited;
            visited.insert(start);
            if (dfs(start, verticesCount, graph, visited)) {
                return true;
            }
        }

        return false;
    }

    bool hasHamiltonianCycle(const std::vector<std::pair<int, int>>& adjList, int verticesCount) {
        // Строим граф
        std::unordered_map<int, std::vector<int>> graph = buildGraph(adjList);

        // Пробуем начать цикл с каждой вершины
        for (int start = 0; start < verticesCount; ++start) {
            std::unordered_set<int> visited;
            visited.insert(start);
            if (dfs(start, start, verticesCount, graph, visited)) {
                return true;
            }
        }

        return false;
    }
};

int main() {
    Graph graph;
    graph.n=4;
    graph.addEdge(1, 2, 5);
    graph.addEdge(2, 3, 6);
    graph.addEdge(2, 4, 3);
    graph.addEdge(4, 3, 2);
    graph.addEdge(3, 4, 7);
    graph.addEdge(4, 1, 2);
    graph.addEdge(3, 1, 2);


    graph.perest_zam();
    graph.brute_force_zam();

    return 0;
}
