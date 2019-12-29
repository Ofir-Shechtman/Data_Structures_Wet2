#ifndef LIST_H
#define LIST_H
#include <iostream>
/*!
 * Doubly linked list of type T
 * @tparam T
 */
template<class T>
class List{
    struct Node;
    Node *head, *tail;
    int list_size;
public:
    List();
    List(const List<T>&);
    ~List();
    List<T>& operator=(const List&);
    class Iterator;
    void erase(Iterator);
    void pop_back();
    void pop_front();
    Iterator insert(const List<T>::Iterator& pos, const T &value);
    Iterator push_back(const T &value);
    Iterator push_front(const T &value);
    const T& front() const;
    const T& back() const;
    const int& size() const;
    void clear();
    Iterator begin() const;
    Iterator end() const;
    bool empty() const;
    class EmptyList : public std::exception{};
};

template<class T>
struct List<T>::Node{
    T data;
    Node* next;
    Node* previous;
    Node(T data, Node* next, Node* previous);
    Node(const Node&)= default;
    ~Node()= default;
};

template<class T>
class List<T>::Iterator{
    Node* node;
public:
    friend class List<T>;
    explicit Iterator(Node* node= nullptr);
    //Iterator(const Iterator& it);
    //~Iterator();
    //Iterator& operator=(const Iterator&);
    const T& operator*() const;
    Iterator& operator++();
    Iterator& operator--();
    bool operator!=(const Iterator& it) const;
    class InvalidIterator : public std::exception{};
};


template<class T>
List<T>::Node::Node(T data, Node *next, Node *previous):data(data), next(next),previous(previous){}

/*!
 * erase from the list the item of the iterator it
 * @tparam T
 * @param it
 */
template<class T>
void List<T>::erase(List::Iterator it) {
    if(!it.node)
        throw typename List<T>::Iterator::InvalidIterator();
    Node* n=it.node;
    if(n->next)
        n->next->previous=n->previous;
    else
        tail=n->previous;
    if(n->previous)
        n->previous->next=n->next;
    else
        head=n->next;
    delete n;
    --list_size;
}

/*!
 * initialize emply list
 * @tparam T
 */
template<class T>
List<T>::List():head(nullptr), tail(nullptr), list_size(0){}



template<class T>
List<T>::~List() {
    clear();
}

/*!
 * insert value to the list *after* the iterator pos
 * @tparam T
 * @param pos
 * @param value
 * @return iterator the the new element
 */
template<class T>
typename List<T>::Iterator List<T>::insert(const List<T>::Iterator &pos, const T &value) {
    if(!pos.node)
        throw typename List<T>::Iterator::InvalidIterator();
    auto *node= new Node(value, pos.node->next, pos.node);
    pos.node->next=node;
    if(node->next)
        node->next->previous=node;
    else
        tail=node;
    ++list_size;
    return Iterator(node);
}

/*!
 * insert value to the end of the list
 * @tparam T
 * @param value
 * @return iterator the the new element
 */
template<class T>
typename List<T>::Iterator List<T>::push_back(const T &value) {
    if(empty())
        return push_front(value);
    return insert(Iterator(tail), value);
}

/*!
 * insert value to the start of the list
 * @tparam T
 * @param value
 * @return iterator the the new element
 */
template<class T>
typename List<T>::Iterator List<T>::push_front(const T &value) {
    auto* node= new Node(value, head, nullptr);
    if(!empty())
        head->previous=node;
    else
        tail=node;
    head=node;
    ++list_size;
    return Iterator(head);
}

/*!
 * remove the last element
 * @tparam T
 */
template<class T>
void List<T>::pop_back() {
    if(empty())
        throw EmptyList();
    erase(Iterator(tail));
}

/*!
 * remove the first element
 * @tparam T
 */
template<class T>
void List<T>::pop_front() {
    if(empty())
        throw EmptyList();
    erase(Iterator(head));
}

/*!
 * @tparam T
 * @return return true if the list is empty, else false
 */
template<class T>
bool List<T>::empty() const{
    return tail==nullptr;
}

/*!
 * returns the number of elements of the list
 */
template<class T>
const int & List<T>::size() const {
    return list_size;
}

/*!
 * @tparam T
 * @return returns iterator to the begin of the list
 */
template<class T>
typename List<T>::Iterator List<T>::begin() const {
    return Iterator(head);
}

/*!
 * @tparam T
 * @return returns iterator to the end of the list
 */
template<class T>
typename List<T>::Iterator List<T>::end() const{
    return Iterator(nullptr);
}

/*!
 * @tparam T
 * @return returns the first value in the list
 */
template<class T>
const T& List<T>::front() const{
    if(empty())
        throw EmptyList();
    return head->data;
}

/*!
 * @tparam T
 * @return returns the last value in the list
 */
template<class T>
const T& List<T>::back() const{
    if(empty())
        throw EmptyList();
    return tail->data;
}

template<class T>
List<T>::List(const List& list) : head(nullptr), tail(nullptr), list_size(0){
    *this=list;
}

template<class T>
List<T> &List<T>::operator=(const List& list) {
    if(this==&list)
        return *this;
    clear();
    for(auto &i : list)
        push_back(i);
    return *this;
}

/*!
 * removes all list's elements
 * @tparam T
 */
template<class T>
void List<T>::clear() {
    while(head){
        pop_front();
    }
}


template<class T>
List<T>::Iterator::Iterator(Node *node) : node(node) {}

/*!
 * returns the data that the iterator points on
 * @tparam T
 * @return
 */
template<class T>
const T& List<T>::Iterator::operator*() const {
    return node->data;
}
/*
template<class T>
List<T>::Iterator::Iterator(const List<T>::Iterator &it) : node(it.node ? new Node(*it.node): nullptr){}

template<class T>
List<T>::Iterator::~Iterator() {
    delete node;
}

template<class T>
typename List<T>::Iterator &List<T>::Iterator::operator=(const List<T>::Iterator& it) {
    delete node;
    node= node ? new Node(*it.node) : nullptr;
}
*/

/*!
 * increase the iterator to the next value
 * @tparam T
 * @return
 */
template<class T>
typename List<T>::Iterator &List<T>::Iterator::operator++() {
    if(!node)
        throw typename List<T>::Iterator::InvalidIterator();
    node=node->next;
    return *this;
}

/*!
 * decrease the iterator to the previous value
 * @tparam T
 * @return
 */
template<class T>
typename List<T>::Iterator &List<T>::Iterator::operator--() {
    if(!node)
        throw typename List<T>::Iterator::InvalidIterator();
    node=node->previous;
    return *this;
}

/*!
 * compare the iterator with another iterator
 * @tparam T
 * @param it
 * @return true if they are equal. else false.
 */
template<class T>
bool List<T>::Iterator::operator!=(const List<T>::Iterator& it) const {
    return node!=it.node;
}


#endif //LIST_H
