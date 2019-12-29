#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include "Stack.h"
#include "Array.h"

using namespace std;

template <class K>
class Compare{
public:
    Compare()= default;
    virtual ~Compare()= default;
    virtual bool operator()(const K& key1, const K& key2) const {return key1<key2;};
};

template <class K, class T>
class AVLTree{
    struct Node;
    Node *root;
    Compare<K>* compare;
    Compare<K>& cmp;
    unsigned int tree_size;
    Node *find_req(const K &key, Node *n, Stack<Node*>* s= nullptr) const;
    Node *insert_req(const K &key, const T &data, Node* n, Stack<Node*>* s);
    Array<Node> to_array();
    static Node *arr2tree(AVLTree::Node* arr, int n);
    static void balance_tree(Stack<Node*>*);
public:
    class Iterator;
    Iterator begin() const;
    Iterator end() const;
    explicit AVLTree(Compare<K>* compare): root(nullptr), compare(compare), cmp(*compare), tree_size(0){}
    AVLTree(const AVLTree &tree);
    AVLTree(const AVLTree &t1, const AVLTree &t2);
    AVLTree<K, T>& operator=(const AVLTree & tree);
    ~AVLTree();
    Iterator find(const K& key) const;
    Iterator insert(const K& key, const T& data=T());
    void erase(const K& key);
    bool empty() const;
    int size() const;
    void clear();
    class KeyNotExists : public exception{};
    class KeyAlreadyExists : public exception{};
 };

template <class K, class T>
struct AVLTree<K,T>::Node{
    K key;
    T data;
    unsigned int height;
    Node* right;
    Node* left;
    explicit Node(K key, T data, unsigned int height = 0,
                  Node *right = nullptr, Node *left = nullptr):
            key(key), data(data),height(height), right(right), left(left){}
    ~Node();
    Node(const Node &node)= delete;
    Node& operator=(const Node& node)=delete;
    int BF();
    void balance();
    void LL();
    void LR();
    void RR();
    void RL();
    void swap_node(Node*);
    void update_height();
};


template <class K, class T>
class AVLTree<K,T>::Iterator{
    Node* node;
    Stack<Node*> stack;
    void Left();
    void Right();
    void Father();
public:
    friend AVLTree;
    Iterator(Node* node, Stack<Node*> stack);
    const K& key() const;
    const T& data() const;
    T& data();
    Iterator operator++();
    bool operator!=(const Iterator& it) const;
    class InvalidIterator : public exception{};
};

template<class K, class T>
typename AVLTree<K,T>::Node *AVLTree<K, T>::find_req(const K &key, AVLTree::Node *n,
                                       Stack<AVLTree::Node *> *s) const{
    if(n == nullptr)
        return nullptr;
    if(cmp(key, n->key)){
        if(s)
            s->push(n);
        return find_req(key, n->left, s);
    }
    else if(cmp(n->key, key)){
        if(s)
            s->push(n);
        return find_req(key, n->right, s);
    }
    return n;
}


template <class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K,T>::find(const K &key) const {
    Stack<Node*> s;
    Node *n = find_req(key, root, &s);
    if(!n){
        throw KeyNotExists();
    }
    return Iterator(n, s);
}

template<class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K, T>::begin() const {
    Stack<Node *> s;
    auto node = root;
    if(node){
        while (node->left){
            s.push(node);
            node = node->left;
        }
    }
    return AVLTree::Iterator(node, s);
}

template<class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K, T>::end() const {
    return AVLTree::Iterator(nullptr, Stack<Node*>());
}

template <class K, class T>
void AVLTree<K,T>::Iterator::Left() {
    stack.push(node);
    node=node->left;
}

template <class K, class T>
void AVLTree<K,T>::Iterator::Right() {
    stack.push(node);
    node=node->right;
}

template <class K, class T>
void AVLTree<K,T>::Iterator::Father() {
    node= stack.top();
    stack.pop();
}

template <class K, class T>
AVLTree<K,T>::Iterator::Iterator(Node* node, Stack<Node*> stack):node(node), stack(stack){
    if(!node)
        return;
}

template <class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K,T>::Iterator::operator++(){
    if(node== nullptr)
        throw InvalidIterator();
    if(node->right){
        Right();
        while(node->left)
            Left();
        return *this;
    }
    while(true) {
        if (stack.empty()) {
            node = nullptr;
            return *this;
        }
        if(stack.top()->left==node){
            Father();
            return *this;
        }
        Father();
    }
}


template <class K, class T>
bool AVLTree<K,T>::Iterator::operator!=(const AVLTree<K, T>::Iterator& it) const {
    return node!=it.node;
}

template<class K, class T>
const K &AVLTree<K, T>::Iterator::key() const{
    return node->key;
}

template<class K, class T>
const T &AVLTree<K, T>::Iterator::data() const {
    return node->data;
}

template<class K, class T>
T &AVLTree<K, T>::Iterator::data() {
    return node->data;
}


template <class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K,T>::insert(const K &key, const T &data) {
    if(root == nullptr){
        root = new Node(key,data);
        tree_size++;
        return Iterator(root, Stack<Node*>());
    }
    Stack<Node*> s;
    AVLTree::Node *new_node = insert_req(key, data, root, &s);
    balance_tree(&s);
    return Iterator(new_node, s);
}

template <class K, class T>
typename AVLTree<K,T>::Node *AVLTree<K,T>::insert_req(const K &key, const T &data, AVLTree::Node* n,
                                                      Stack<AVLTree::Node*>* s) {
    s->push(n);
    if(cmp(key, n->key)){
        if(n->left == nullptr){
            n->left = new Node(key, data);
            tree_size++;
            return n->left;
        }
        else{
            return insert_req(key, data, n->left, s);
        }
    }
    else if(cmp(n->key, key)){
        if(n->right == nullptr){
            n->right = new Node(key, data);
            tree_size++;
            return n->right;
        }
        else {
            return insert_req(key, data, n->right, s);
        }
    }
    throw KeyAlreadyExists();
    //return nullptr;
}


template <class K, class T>
void AVLTree<K,T>::Node::balance() {
    if (BF() == 2 && left->BF() >= 0) {
        LL();
    }
    else if (BF() == 2 && left->BF() == -1){
        LR();
    }
    else if (BF() == -2 && right->BF() <= 0){
        RR();
    }
    else if (BF() == -2 && right->BF() == 1){
        RL();
    }
    update_height();
}

template <class K, class T>
int AVLTree<K,T>::Node::BF() {
    int l = left ? left->height : -1;
    int r = right ? right->height : -1;
    return l - r;
}

template <class K, class T>
void AVLTree<K,T>::Node::LL() {
    AVLTree::Node *left_son, *left_left_son, *left_right_son, *right_son;
    left_son = left;
    left_left_son = left->left;
    left_right_son = left->right;
    right_son = right;

    left = left_left_son;
    right = left_son;
    left_son->left = left_right_son;
    left_son->right = right_son;
    swap_node(left_son);
    left_son->update_height();
}
template <class K, class T>
void AVLTree<K,T>::Node::RR() {
    AVLTree::Node *right_son, *right_left_son, *right_right_son, *left_son;
    right_son = right;
    right_left_son = right->left;
    right_right_son = right->right;
    left_son = left;

    right = right_right_son;
    left = right_son;
    right_son->left = left_son;
    right_son->right = right_left_son;
    swap_node(right_son);
    right_son->update_height();
}

template <class K, class T>
void AVLTree<K,T>::Node::LR() {
    left->RR();
    LL();
}

template <class K, class T>
void AVLTree<K,T>::Node::RL() {
    right->LL();
    RR();
}

template<class K, class T>
void AVLTree<K, T>::Node::update_height() {
    int l = left ? left->height : -1;
    int r = right ? right->height : -1;
    height=max(l, r)+1;
}

template<class K, class T>
AVLTree<K, T>::Node::~Node() {
    delete left;
    delete right;
}

template<class K, class T>
void AVLTree<K, T>::Node::swap_node(AVLTree<K, T>::Node* node) {
    swap(key, node->key);
    swap(data, node->data);
}

template<class K, class T>
bool AVLTree<K, T>::empty() const{
    return root == nullptr;
}

template<class K, class T>
void AVLTree<K, T>::erase(const K &key) {
    Stack<Node*> s;
    Node* n=find_req(key, root, &s);
    if(!n)
        throw KeyNotExists();
    Node* father= s.empty() ? nullptr : s.top();
    //CASE 1: no children
    if(!n->right && !n->left){
        if(father)
            father->left==n ? father->left= nullptr : father->right= nullptr;
        else
            root= nullptr;
        delete n;
    }
    //CASE 2: one child
    else if(!n->right || !n->left){
        if(n->right){
            n->swap_node(n->right);
            delete n->right;
            n->right= nullptr;
        }
        else{
            n->swap_node(n->left);
            delete n->left;
            n->left= nullptr;
        }
        s.push(n);
    }
    //CASE 3: has 2 children
    else {
        Node *prev = n->left;
        s.push(n);
        while (prev->right) {
            s.push(prev);
            prev = prev->right;
        }
        n->swap_node(prev);
        //CASE 3.2 his previous has left child
        if(prev->left){
            prev->swap_node(prev->left);
            delete prev->left;
            prev->left= nullptr;
            s.push(prev);
        }
        //CASE 3.1 his previous has no children
        else{
            father = s.top();
            father->left==prev ? father->left= nullptr : father->right= nullptr;
            delete prev;
        }
    }
    balance_tree(&s);
    --tree_size;

}

template<class K, class T>
int AVLTree<K, T>::size() const {
    return tree_size;
}

template<class K, class T>
void AVLTree<K, T>::clear() {
    delete root;
    root= nullptr;
    tree_size=0;

}

template<class K, class T>
AVLTree<K, T>::AVLTree(const AVLTree &tree) :compare(tree.compare), cmp(*compare), tree_size(0) {
    *this=tree;

}

template<class K, class T>
AVLTree<K, T> &AVLTree<K, T>::operator=(const AVLTree &tree) {
    compare=tree.compare;
    cmp=*compare;
    clear();
    for(auto i=tree.begin(); i!=tree.end(); ++i)
        insert(i.key(), i.data());
    return *this;
}

template<class K, class T>
AVLTree<K, T>::~AVLTree() {
    clear();
}

template<class K, class T>
AVLTree<K, T>::AVLTree(const AVLTree &t1, const AVLTree &t2):
    compare(t1.compare), cmp(*compare), tree_size(t1.size()+t2.size())
{
    Array<Node> a1=t2.to_array(), a2=t2.to_array();
    Array<Node> a3 = Array<Node>(a1, a2);
    root = arr2tree(*a3, a3.size());
}


template<class K, class T>
typename AVLTree<K, T>::Node* AVLTree<K, T>::arr2tree(AVLTree::Node *arr, int n) {
    Node* root;
    if(n==0)
        return nullptr;
    if(n==1){
        root=arr[0];
        return root;
    }
    root=arr[n/2];
    root->left=arr2tree(arr, n/2);
    root->right=arr2tree(arr+n/2+1, n-n/2-1);
    return root;
}


template<class K, class T>
Array<typename AVLTree<K,T>::Node> AVLTree<K, T>::to_array() {
    Array<AVLTree<K,T>::Node> arr(tree_size);
    int i=0;
    for(auto it=begin(); it!=end(); ++it){
        auto node=it.node;
        node.left= nullptr;
        node.right= nullptr;
        arr[i++]=node;
    }
    return arr;
}

template<class K, class T>
void AVLTree<K, T>::balance_tree(Stack<AVLTree::Node *> *s) {
    while (!s->empty()) {
        Node *n = s->top();
        s->pop();
        n->balance();
    }
}



#endif //AVLTREE_H
