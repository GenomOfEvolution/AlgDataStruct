#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <queue>
#include <string>

// Общими требованиями к лабораторной работе являются :
// 1) вводить  граф из файла в понятной для пользователя форме
// (не в виде матрицы смежности, без дублирования информации и т. п.);
// 2) обеспечить   возможность   многократных   запросов   без
// повторного запуска программы.


// 26. Имеется сеть автомобильных дорог.По  некоторым дорогам
// можно  проехать  только  в одном  направлении.Известна  длина
// каждой дороги, причем она может быть разной в  зависимости  от
// направления.Один  из  городов  является  столицей.Требуется
// вывести список длин вторых по минимальности путей из столицы в
// другие города.Допускается присутствие циклических путей(12).

/*
Решение.
В вершинах по две числовые метки.
Первая для нахождения кратчайшего пути, а вторая - для второго по минимальности пути.
Первая метка может получаться только из первых меток, а вторая метка может быть найдена из предыдущих меток обоих типов.
В скобках после каждой метки для наглядности указываются два числа.
Первое число определяет номер предыдущей вершины, а второе – тип предыдущей метки, первый или второй, на основании которой была вычислена текущая метка.
*/

const int INF = 10000000;

std::vector<int> DjikstraWithSecondPath(const std::vector<std::vector<std::pair<int, int>>>& g, int src) 
{
    int n = (int)(g.size());
    std::vector<int> d(n, INF), d2(n, INF); 
    std::vector<std::pair<int, int>> p(n, { -1, -1 }); 
    d[src] = 0;

    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<std::pair<int, std::pair<int, int>>>> q;
    q.push({ 0, { src, 1 } }); 

    while (!q.empty()) 
    {
        auto [du, pair] = q.top();
        auto [u, labelType] = pair;
        q.pop();

        if ((labelType == 1 && du != d[u]) || (labelType == 2 && du != d2[u])) 
        {
            continue;
        }

        for (auto& [w, v] : g[u]) 
        {
            if (du + w <= d[v]) 
            {
                d2[v] = d[v];
                d[v] = du + w;
                p[v] = { u, 1 }; 
                q.push({ d[v], { v, 1 } });
                q.push({ d2[v], { v, 2 } }); 
            }
            else if (du + w > d[v] && du + w < d2[v]) 
            {
                d2[v] = du + w;
                p[v] = { u, 2 }; 
                q.push({ d2[v], {v, 2} });
            }
        }
    }

    return d2;
}

std::vector<std::vector<std::pair<int, int>>> ReadGraph(std::ifstream& input)
{
    std::vector<std::vector<std::pair<int, int>>> graph;

    int u, v, w;
    int maxIndex = -1;
    while (input >> u >> v >> w)
    {
        maxIndex = std::max(maxIndex, std::max(u, v));

        if (maxIndex >= graph.size())
        {
            graph.resize(maxIndex + 1);
        }

        graph[u].push_back(std::make_pair(w, v));
    }

    return graph;
}

void PrintPaths(std::ofstream& output, const std::vector<int>& paths)
{
    
    for (int i = 2; i < paths.size(); i++)
    {
        if (paths[i] == INF)
        {
            output << "No\n";
        }
        else
        {
            output << paths[i] << "\n";
        }
    }
}

int main()
{
    setlocale(LC_ALL, "RU");

	std::ifstream input;
    std::ofstream outfile;

    std::string inputName;
    std::string outputName;
    std::string answ = "y";

    do
    {
        std::cout << "Введите имя входного файла: ";
        std::cin >> inputName;

        input.open(inputName);

        if (!input)
        {
            std::cout << "Can't open " << inputName << "\n";
            continue;
        }

        std::vector<std::vector<std::pair<int, int>>> graph = ReadGraph(input);
        std::vector<int> shortestPaths = DjikstraWithSecondPath(graph, 1);

        std::cout << "Введите имя выходного файла: ";
        std::cin >> outputName;

        outfile.open(outputName, std::ios::trunc);
        PrintPaths(outfile, shortestPaths);

        std::cout << "Еще раз? (y/n): ";
        std::cin >> answ;

    } while (answ == "Y" || answ == "y");

	return EXIT_SUCCESS;
}
