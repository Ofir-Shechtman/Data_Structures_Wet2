#ifndef STACK_H
#define STACK_H

#include"List.h"

template<class T>
class Stack {
    List<T> list;
public:
    void push(const T&);
    void pop();
    const T& top() const;
    bool empty() const;
    int size() const;
    class EmptyStack : public List<T>::EmptyList{};
};


template<class T>
void Stack<T>::push(const T& value) {
    list.push_back(value);
}

template<class T>
void Stack<T>::pop() {
    if(empty())
        throw EmptyStack();
    list.pop_back();
}

template<class T>
const T& Stack<T>::top() const{
    if(empty())
        throw EmptyStack();
    return list.back();
}

template<class T>
bool Stack<T>::empty() const {
    return list.empty();
}

template<class T>
int Stack<T>::size() const {
    return list.size();
}


#endif //STACK_H
