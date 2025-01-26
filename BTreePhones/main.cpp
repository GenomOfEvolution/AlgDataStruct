// 22. Составить   программу  поиска  записи  с  включением  в
// сильно ветвящемся  Б - дереве  порядка N.Б - дерево  хранится  и
// обрабатывается в оперативной памяти(12).
// Автор: Нечаев Олег ПС-24
// Среда: MS VisualStudio 2022

#include <iostream>

const int MAX_KEYS = 3;

struct Node
{
    int numberOfKeys; 
    int position = -1; 
    int keys[MAX_KEYS];
    Node* children[MAX_KEYS + 1];

    Node();
    int Insert(int value);
    Node* Split(Node* node, int* median);
    void Print();
    void PrintUtil(int height, bool checkRoot);
    int getHeight();
    ~Node();
};

Node::Node()
{
    numberOfKeys = 0;
    for (int i = 0; i < MAX_KEYS; i++)
    {
        children[i] = nullptr;
        keys[i] = -1;
    }
    children[MAX_KEYS] = nullptr;
}

int Node::Insert(int value)
{
    // Проверка, есть ли элемент уже в текущем узле
    for (int i = 0; i < numberOfKeys; i++)
    {
        if (keys[i] == value)
        {
            std::cout << "Element " << value << " already exists in the tree." << std::endl;
            return 0; // Элемент уже существует, вставка не требуется
        }
    }

    if (children[0] == nullptr)
    {
        if (numberOfKeys >= MAX_KEYS)
        {
            return 1; 
        }

        // Вставка значения в текущий узел (лист)
        keys[++position] = value;
        ++numberOfKeys;

        // Сортировка ключей в узле
        for (int i = position; i > 0 && keys[i] < keys[i - 1]; --i)
        {
            std::swap(keys[i], keys[i - 1]);
        }
    }
    else
    {
        // Поиск позиции для вставки в дочерний узел
        int i = 0;
        while (i < numberOfKeys && value > keys[i])
        {
            i++;
        }

        // Рекурсивная вставка в дочерний узел
        int check = children[i]->Insert(value);

        // Если дочерний узел переполнен, выполняем разделение
        if (check)
        {
            // Проверка, что текущий узел не переполнен
            if (numberOfKeys >= MAX_KEYS)
            {
                return 1; // Узел переполнен
            }

            int mid;
            Node* newNode = Split(children[i], &mid);

            // Поиск позиции для вставки медианы в текущий узел
            int insertPos = 0;
            while (insertPos < numberOfKeys && mid > keys[insertPos])
            {
                insertPos++;
            }

            // Сдвиг ключей для вставки медианы
            if (numberOfKeys < MAX_KEYS)
            {
                for (int j = numberOfKeys; j > insertPos; --j)
                {
                    keys[j] = keys[j - 1];
                }
                keys[insertPos] = mid;
                ++numberOfKeys;
                ++position;
            }

            // Сдвиг дочерних узлов для вставки нового узла
            if (numberOfKeys <= MAX_KEYS)
            {
                for (int j = numberOfKeys; j > i + 1; --j)
                {
                    children[j] = children[j - 1];
                }
                children[i + 1] = newNode;
            }
        }
    }

    // Возвращаем 1, если узел переполнен, иначе 0
    return (numberOfKeys == MAX_KEYS) ? 1 : 0;
}

Node* Node::Split(Node* node, int* median) 
{
    int NumberOfKeys = node->numberOfKeys;
    auto* newNode = new Node();
    int midValue = NumberOfKeys / 2;
    *median = node->keys[midValue];
    int i;

    for (i = midValue + 1; i < NumberOfKeys; i++)
    {
        newNode->keys[++newNode->position] = node->keys[i];
        newNode->children[newNode->position] = node->children[i];
        newNode->numberOfKeys++;
        node->position--;
        node->numberOfKeys--;
        node->children[i] = nullptr;
    }
    newNode->children[newNode->position + 1] = node->children[i];
    node->children[i] = nullptr;

    node->numberOfKeys--; 
    node->position--;
    return newNode;
}

void Node::Print()
{
    int height = getHeight();
    for (int i = 1; i <= height; i++) 
    {
        if (i == 1)
            PrintUtil(i, true);
        else
            PrintUtil(i, false);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Node::PrintUtil(int height, bool checkRoot)
{
    if (height == 1 || checkRoot)
    {
        for (int i = 0; i < numberOfKeys; i++)
        {
            std::cout << keys[i] << " ";
            if (i == numberOfKeys - 1)
            {
                std::cout << "\t";
            }
        }
    }
    else
    {
        for (int i = 0; i <= numberOfKeys; i++)
        {
            children[i]->PrintUtil(height - 1, false);
        }
    }
}

int Node::getHeight()
{
    int COUNT = 1;
    Node* Current = this;
    while (true)
    {
        if (Current->children[0] == nullptr)
            return COUNT;
        Current = Current->children[0];
        COUNT++;
    }
}

Node::~Node()
{
    for (int i = 0; i <= numberOfKeys; i++)
    {
        delete children[i];
    }
}

class BTree
{
private:
    Node* root;
    int count = 0; 

public:
    BTree();
    void Insert(int value);
    void Print() const;
    ~BTree();
};

BTree::BTree()
{
    root = nullptr;
    count = 0;
}

void BTree::Insert(int value)
{
    count++;

    if (root == nullptr)
    {
        root = new Node();
        root->keys[++root->position] = value;
        root->numberOfKeys = 1;
    }
    else
    {
        int check = root->Insert(value);
        if (check)
        {
            int mid;
            Node* splitNode = root->Split(root, &mid);
            auto* newNode = new Node();
            newNode->keys[++newNode->position] = mid;
            newNode->numberOfKeys = 1;
            newNode->children[0] = root;
            newNode->children[1] = splitNode;
            root = newNode;
        }
    }
}

void BTree::Print() const
{
    if (root != nullptr)
    {
        root->Print();
    }
    else
    {
        std::cout << "The B-Tree is Empty" << std::endl;
    }
}

BTree::~BTree()
{
    delete root;
}

int main()
{
   /* BTree t1;
    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);
    t1.Print(); */

    BTree t;

    int key = 0;
    while (key >= 0)
    {
        std::cout << "Enter key (negative for exit): ";
        std::cin >> key;
        t.Insert(key);
        t.Print();
        std::cout << "\n";
    }

    return EXIT_SUCCESS;
}