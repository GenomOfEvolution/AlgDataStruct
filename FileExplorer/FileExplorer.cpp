#include "FileExplorer.h"

FileExplorer::FileExplorer()
{
    ROOT = new Tree;
    ROOT->name = "ROOT";
    ROOT->depth = -1;

    buffer = nullptr;
}

FileExplorer::FileExplorer(Args args)
{
    ROOT = new Tree;
    ROOT->name = "ROOT";
    ROOT->depth = -1;
  
    LoadTree(args.inputFileName);
}

void FileExplorer::LoadFromFile(std::ifstream& input, Tree*& root)
{
    std::string line;

    Tree* curElem = nullptr;        // указатель на предыдущую вершину
    size_t prevDepth = -1;          // уровень предыдущей вершины
    Tree* curElemParent = nullptr;  // отец вводимой вершины

    while (std::getline(input, line))
    {
        if (line.length() == 0)
        {
            continue;
        }

        // Считываем глубину текущего элемента
        int curDepth = 0;
        bool isFile = (line[curDepth] == depthFileDelimetr);

        while (line[curDepth] == depthFolderDelimetr || line[curDepth] == depthFileDelimetr)
        {
            curDepth++;
        }

        Tree* newElem = new Tree;
        newElem->depth = curDepth;
        newElem->name = line.substr(curDepth);
        newElem->isFile = isFile;

        if (curDepth == 0)
        {
            root = newElem;
            curElem = root;
            prevDepth = 0;

            continue;
        }

        if (curDepth > prevDepth)
        {
            curElem->children.push_back(newElem);
            newElem->parent = curElem;
        }
        else if (curDepth == prevDepth)
        {
            curElemParent = curElem->parent;
            curElemParent->children.push_back(newElem);
            newElem->parent = curElemParent;
        }
        else
        {
            curElemParent = curElem;
            for (int i = 0; i <= (prevDepth - curDepth); i++)
            {
                curElemParent = curElemParent->parent;
            }
            curElemParent->children.push_back(newElem);
            newElem->parent = curElemParent;
        }
        prevDepth = curDepth;
        curElem = newElem;
    }
}

void FileExplorer::LoadTree(std::string fileName)
{
    std::ifstream input(fileName);
    if (!input)
    {
       std::cout << "Can't open " + fileName + "!\n";
       return;
    }

    ROOT->children.push_back(new Tree);
    // Загружаем новый диск в компьютер
    FileExplorer::LoadFromFile(input, ROOT->children[ROOT->children.size() - 1]);
    input.close();
    ROOT->children[ROOT->children.size() - 1]->parent = ROOT;
}

void FileExplorer::ChangeName(Tree*& node, const std::string& newName)
{
    node->name = newName;
}

void FileExplorer::CopyElem(Tree* node)
{
    isExtractMode = false;
    DeleteTree(buffer);
    buffer = CopyTree(node);
}

void FileExplorer::ExtractElem(Tree*& node)
{
    isExtractMode = true;
    DeleteTree(buffer);
    buffer = CopyTree(node);
    DeleteTree(node);
}

Tree* FileExplorer::CopyTree(const Tree* root) 
{
    if (root == nullptr) 
    {
        return nullptr;
    }

    Tree* newNode = new Tree(root->isFile, root->name, root->depth, root->parent);

    for (const Tree* child : root->children) 
    {
        Tree* childCopy = CopyTree(child);
        childCopy->parent = newNode; 
        newNode->children.push_back(childCopy);
    }

    return newNode;
}
void FileExplorer::AddNewElem(bool isFile, std::string& name, Tree*& parent)
{
    Tree* newElem = new Tree;
    newElem->depth = parent->depth + 1;
    newElem->isFile = isFile;
    newElem->name = name;
    newElem->parent = parent;
    parent->children.push_back(newElem);
}

void FileExplorer::PasteNode(Tree*& curNode)
{
    if (buffer == nullptr)
    {
        return;
    }

    if (isExtractMode)
    {
        curNode->children.push_back(CopyTree(buffer));
        isExtractMode = false;
        DeleteTree(buffer);
        buffer = nullptr;
    }
    else
    {
        curNode->children.push_back(buffer);
        buffer = CopyTree(buffer);
    }
}

void FileExplorer::ShowBuffer()
{
    if (buffer != nullptr)
    {
        std::cout << "\nBuffer: " << buffer->name;
    }
    else
    {
        std::cout << "\nBuffer is empty!";
    }
}

void FileExplorer::SaveNode(Tree* node, std::ofstream& output)
{
    char outSymbol = (node->isFile) ? depthFileDelimetr : depthFolderDelimetr;
    for (int i = 0; i < node->depth; i++)
    {
        output << outSymbol;
    }
    output << node->name << "\n";
    for (auto it : node->children)
    {
        SaveNode(it, output);
    }
}

void FileExplorer::SaveTree(std::ofstream& output)
{
    for (auto it : ROOT->children)
    {
        SaveNode(it, output);
    }
}

void DeleteNode(Tree*& node)
{
    if (node != nullptr)
    {
        for (Tree* child : node->children)
        {
            DeleteNode(child);
        }
        delete node;
    }
}

void FileExplorer::DeleteTree(Tree*& node)
{
    if (node == ROOT || node == nullptr)
    {
        return;
    }

    // Удаляем текущую ноду из памяти родителя
    if (node->parent != nullptr)
    {
        auto index = std::find(node->parent->children.begin(), node->parent->children.end(), node);
        if (index != node->parent->children.end())
        {
            node->parent->children.erase(index);
        }
    }

    DeleteNode(node);
}

Tree::Tree()
{
}
