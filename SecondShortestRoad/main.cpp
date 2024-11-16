#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>

const int INF = 2000000;

struct Edge 
{
    int to;
    int weight;
};

class Graph
{
public:
    void ReadEdges(std::ifstream& input);
    void PrintEdges();
    std::unordered_map<int, int> SecondShortest(int s, std::unordered_map<int, std::vector<int>> paths);

    std::unordered_map<int, std::vector<int>> djikstraPaths(int s);

private:
    std::unordered_map<int, std::vector<Edge>> data;
    void SetWeight(int from, int to, int weight);

    std::unordered_map<int, int> djikstra(int s);
};

struct CompareEdge 
{
    bool operator()(const Edge& a, const Edge& b) const 
    {
        return a.weight < b.weight || (a.weight == b.weight && a.to < b.to);
    }
};

void Graph::ReadEdges(std::ifstream& input)
{
    int left, right, weight;

    while (input)
    {
        input >> left >> right >> weight;
        if (weight < 0)
        {
            throw new std::runtime_error("\nВес ребра должен быть не отрицательным!\n");
        }

        Edge edge;
        edge.to = right;
        edge.weight = weight;

        data[left].push_back(edge);
    }
}

void Graph::PrintEdges()
{
    for (auto i : data)
    {
        std::cout << "Vertex " << i.first << ": \n";
        for (auto j : data[i.first])
        {
            std::cout << "   -> " << j.to << " with weight " << j.weight << "\n";
        }
    }
}

std::unordered_map<int, int> Graph::SecondShortest(int s, std::unordered_map<int, std::vector<int>> paths)
{
    if (data.find(s) == data.end())
    {
        std::string line = "Нет вершины с индексом " + std::to_string(s) + "\n";
        throw std::runtime_error(line);
    }

    std::unordered_map<int, int> dist;

    for (auto i : paths)
    {
        if (i.second.size() < 2)
        {
            continue;
        }

        auto points = i.second;
        int secShortest = INF;
        for (int j = points.size() - 1; j > 0; j--)
        {
            // Находим нужное ребро
            int startIndex = points[j - 1];
            int endIndex = points[j];
            auto p = data[startIndex];

            // Запоминаем его вес
            int weight = 0;
            for (auto k : p)
            {
                if (k.to == endIndex)
                {
                    weight = k.weight;
                    break;
                }
            }

            // Делаем его вес бесконечным
            SetWeight(startIndex, endIndex, INF);

            // Находим кротчайший путь
            auto res = djikstra(s);
            int newWeight = res[i.first];

            if (newWeight < secShortest)
            {
                secShortest = newWeight;
            }

            // Возвращаем вес ребру
            SetWeight(startIndex, endIndex, weight);
        }
        dist[i.first] = secShortest;
    }

    return dist;
}

std::unordered_map<int, std::vector<int>> Graph::djikstraPaths(int s) 
{
    std::unordered_map<int, int> dist;
    std::unordered_map<int, int> predecessors;
    std::unordered_map<int, std::vector<int>> paths;

    for (const auto& pair : data) 
    {
        dist[pair.first] = INF;
    }
    dist[s] = 0;

    std::priority_queue<Edge, std::vector<Edge>, CompareEdge> q;
    q.push(Edge(s, 0));

    while (!q.empty()) 
    {
        Edge edge = q.top();
        q.pop();
        int v = edge.to;

        for (auto& e : data[v]) {
            int to = e.to;
            int len = e.weight;

            if (dist[v] + len < dist[to]) 
            {
                dist[to] = dist[v] + len;
                predecessors[to] = v; 
                q.push(Edge(to, dist[to]));
            }
        }
    }

    // Восстановление путей
    for (const auto& pair : dist) 
    {
        int current = pair.first;
        std::vector<int> path;
        while (current != s && predecessors.find(current) != predecessors.end()) 
        {
            path.push_back(current);
            current = predecessors[current];
        }

        if (current == s) 
        {
            path.push_back(s);
            std::reverse(path.begin(), path.end());
            paths[pair.first] = path;
        }
    }

    return paths;
}

void Graph::SetWeight(int from, int to, int weight)
{
    if (data.find(from) != data.end())
    {
        for (int i = 0; i < data[from].size(); i++)
        {
            if (data[from].at(i).to == to)
            {
                data[from].at(i).weight = weight;
                break;
            }
        }
    }
}

std::unordered_map<int, int> Graph::djikstra(int s)
{
    if (data.find(s) == data.end())
    {
        std::string line = "Нет вершины с индексом " + std::to_string(s) + "\n";
        throw std::runtime_error(line);
    }

    std::unordered_map<int, int> dist;

    for (const auto& pair : data)
    {
        dist[pair.first] = INF;
    }
    dist[s] = 0;

    std::priority_queue<Edge, std::vector<Edge>, CompareEdge> q;
    q.push(Edge(s, 0));

    while (!q.empty())
    {
        Edge edge = q.top();
        q.pop();
        int v = edge.to;

        for (auto& e : data[v])
        {
            int to = e.to;
            int len = e.weight;

            if (dist[v] + len < dist[to])
            {
                dist[to] = dist[v] + len;
                q.push(Edge(to, dist[to]));
            }
        }
    }

    return dist;
}

int main()
{
    setlocale(LC_ALL, "RU");

    Graph g;

    std::ifstream input;
    input.open("input.txt");

    if (!input)
    {
        return EXIT_FAILURE;
    }

    try
    {
        g.ReadEdges(input);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

    try
    {
        std::unordered_map<int, std::vector<int>> res = g.djikstraPaths(1);
        auto secondShortest = g.SecondShortest(1, res);

        for (auto i : secondShortest)
        {
            std::cout << "To " << i.first << " = " << i.second << "\n";
        }
        
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what();
    }

    return EXIT_SUCCESS;
}
