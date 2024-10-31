#include <stdexcept>

template <typename T>
class MyStack 
{
private:
    struct Node 
    {
        T data;
        Node* next = nullptr;   
        Node(T value) : data(value), next(nullptr) {} 
    };

    Node* top;

public:
    MyStack() : top(nullptr) 
    {   } 

    ~MyStack() 
    {
        while (!isEmpty()) 
        {
            pop(); 
        }
    }

    void push(T value) 
    {
        Node* newNode = new Node(value); 
        newNode->next = top; 
        top = newNode; 
    }

    T pop() 
    {
        if (isEmpty()) 
        {
            throw std::runtime_error("Стек пуст. Невозможно удалить элемент."); 
        }

        Node* temp = top; 
        T poppedValue = top->data;
        top = top->next; 

        delete temp; 
        return poppedValue; 
    }

    bool isEmpty()
    {
        return top == nullptr;
    }
};