#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "ArgsParser.h"

struct Tree;
class FileExplorer;

struct Tree
{
    bool isFile = false;
    std::string name = "";
    int depth = -1;
    Tree* parent = nullptr;         
    std::vector<Tree*> children = {};

    Tree(bool isFile, std::string name, int depth, Tree* parent = nullptr) : isFile(isFile), name(name), depth(depth), parent(parent) {}
    Tree();
};

class FileExplorer
{
public:
    FileExplorer();
	FileExplorer(Args args);
    void DeleteTree(Tree*& node);
    void LoadTree(std::string fileName);
    void ChangeName(Tree*& node, const std::string& newName);
    void CopyElem(Tree* node);
    void ExtractElem(Tree*& node);
    void AddNewElem(bool isFile, std::string& name, Tree*& parent);
    void PasteNode(Tree*& curNode);
    void ShowBuffer();
    void SaveTree(std::ofstream& output);
    Tree* ROOT = nullptr;

private:
    void LoadFromFile(std::ifstream& input, Tree*& root);
    void SaveNode(Tree* node, std::ofstream& output);
    
    Tree* CopyTree(const Tree* root);
    Tree* buffer = nullptr;
    bool isExtractMode = false;

    const char depthFolderDelimetr = '\\';
    const char depthFileDelimetr = '*';
};