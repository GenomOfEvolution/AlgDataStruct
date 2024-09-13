#include "MyStack.h"

template <typename T>
void MyStack<T>::push(T value) 
{
    Node* newNode = new Node(value);
    newNode->next = top;
    top = newNode;
}

template <typename T>
T MyStack<T>::pop() 
{
    if (isEmpty()) 
    {
        throw std::runtime_error("Стек пуст");
    }

    T value = top->data;

    Node* temp = top;
    top = top->next;
    delete temp;
    
    return value;
}

template <typename T>
bool MyStack<T>::isEmpty() 
{
    return top == nullptr;
}