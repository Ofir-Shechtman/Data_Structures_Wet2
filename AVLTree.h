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
    virtual Compare* clone(){return new Compare();};
};

template <class K, class T>
class AVLTree{
public: //TODO: remove after tests
    struct Node;
    Node *root;
    Compare<K>* compare;
    Compare<K>& cmp;
    unsigned int tree_size;
    Node *find_rec(const K &key, Node *n, Stack<Node *> *s = nullptr) const;
    Node *insert_rec(const K &key, const T &data, Node *n, Stack<Node *> *s);
    Array<Node> to_array() const;
    static Node *arr2tree(const AVLTree::Node* const arr, int n);
    static void balance_tree(Stack<Node*>*);
public:
    class Iterator;
    Iterator begin() const;
    Iterator end() const;
    explicit AVLTree(Compare<K>* compare): root(nullptr), compare(compare->clone()), cmp(*compare), tree_size(0){}
    AVLTree(const AVLTree &tree);
    AVLTree(const AVLTree &t1, const AVLTree &t2);
    AVLTree<K, T>& operator=(const AVLTree & tree);
    ~AVLTree();
    Iterator find(const K& key) const;
    Iterator find_Kth_element(int k) const;
    int get_sum_less_then(const Iterator&);
    Iterator insert(const K& key, const T& data=T());
    void erase(const Iterator&);
    void erase(const K& key);
    bool empty() const;
    int size() const;
    void clear();
    class KeyNotExists : public exception{};
    class KeyAlreadyExists : public exception{};
 };

template <class K, class T>
struct AVLTree<K,T>::Node{
public: //TODO: remove after tests
    K key;
    T data;
    unsigned int height;
    struct Rank{
        int count;
        T sum;
        explicit Rank(T sum):count(1),sum(sum){}
    } rank;
    Node* right;
    Node* left;
    explicit Node(K key=K(), T data=T(), unsigned int height = 0,
                  Node *right = nullptr, Node *left = nullptr):
            key(key), data(data),height(height), rank(Rank(data)),
            right(right), left(left){}
    ~Node();
    //Node(const Node &node)= delete;
    //Node& operator=(const Node& node)=delete;
    int BF();
    void balance();
    void LL();
    void LR();
    void RR();
    void RL();
    void swap_node(Node*);
    void update_height();
    void update_rank();
};


template <class K, class T>
class AVLTree<K,T>::Iterator{
    Node* node;
    Stack<Node*> stack;
    void Left();
    void Right();
    void Father();
public:
    friend AVLTree<K,T>;
    explicit Iterator(Node* node= nullptr, Stack<Node*> stack = Stack<Node*>());
    const K& key() const;
    const T& data() const;
    T& data();
    Iterator operator++();
    bool operator!=(const Iterator& it) const;
    class InvalidIterator : public exception{};
};

template<class K, class T>
typename AVLTree<K,T>::Node *AVLTree<K, T>::find_rec(const K &key,
                                                     AVLTree::Node *n,
                                                     Stack<AVLTree::Node *> *s) const{
    if(n == nullptr)
        return nullptr;
    if(cmp(key, n->key)){
        if(s)
            s->push(n);
        return find_rec(key, n->left, s);
    }
    else if(cmp(n->key, key)){
        if(s)
            s->push(n);
        return find_rec(key, n->right, s);
    }
    return n;
}


template <class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K,T>::find(const K &key) const {
    Stack<Node*> s;
    Node *n = find_rec(key, root, &s);
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
    AVLTree::Node *new_node = insert_rec(key, data, root, &s);
    balance_tree(&s);
    return Iterator(new_node, s);
}

template <class K, class T>
typename AVLTree<K,T>::Node *AVLTree<K,T>::insert_rec(const K &key,
                                                      const T &data,
                                                      AVLTree::Node *n,
                                                      Stack<AVLTree::Node *> *s) {
    s->push(n);
    if(cmp(key, n->key)){
        if(n->left == nullptr){
            n->left = new Node(key, data);
            tree_size++;
            return n->left;
        }
        else{
            return insert_rec(key, data, n->left, s);
        }
    }
    else if(cmp(n->key, key)){
        if(n->right == nullptr){
            n->right = new Node(key, data);
            tree_size++;
            return n->right;
        }
        else {
            return insert_rec(key, data, n->right, s);
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
    update_rank();
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
    left_son->update_rank();
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
    right_son->update_rank();
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
void AVLTree<K, T>::Node::update_rank() {
    int left_count= left ? left->rank.count : 0;
    int right_count= right ? right->rank.count : 0;
    rank.count= 1  + left_count + right_count;
    T left_sum= left ? left->rank.sum : 0;
    T right_sum= right ? right->rank.sum : 0;
    rank.sum= data + left_sum + right_sum;

}


template<class K, class T>
bool AVLTree<K, T>::empty() const{
    return root == nullptr;
}


template<class K, class T>
void AVLTree<K, T>::erase(const AVLTree::Iterator &it) {
    Stack<Node*> s = it.stack;
    Node* n= it.node;
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
AVLTree<K, T>::AVLTree(const AVLTree &tree) :root(nullptr), compare(tree.compare->clone()),
    cmp(*compare), tree_size(0) {
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
    delete compare;
}

template<class K, class T>
AVLTree<K, T>::AVLTree(const AVLTree &t1, const AVLTree &t2):
    compare(t1.compare), cmp(*compare), tree_size(t1.size()+t2.size())
{
    Array<Node> arr1=t2.to_array(), arr2=t2.to_array();
    unsigned int n1=arr1.size(), n2=arr2.size();
    Array<Node> arr3 = Array<Node>(n1+n2);

    unsigned int i = 0, j = 0, k = 0;
    while (i<n1 && j <n2)
    {
        if (cmp(arr1[i].key,arr2[j].key))
            arr3[k++] = arr1[i++];
        else
            arr3[k++] = arr2[j++];
    }

    // Store remaining elements of first array
    while (i < n1)
        arr3[k++] = arr1[i++];

    // Store remaining elements of second array
    while (j < n2)
        arr3[k++] = arr2[j++];
    root = arr2tree(*arr3, arr3.size());
}


template<class K, class T>
typename AVLTree<K, T>::Node* AVLTree<K, T>::arr2tree(const AVLTree::Node * const arr, int n) {
    Node* root;
    if(n==0)
        return nullptr;
    if(n==1){
        root= new Node(arr[0]);
        root->rank.count=1;
        root->rank.sum=root->data;
        return root;
    }
    root= new Node(arr[n/2]);
    root->left=arr2tree(arr, n/2);
    root->right=arr2tree(arr+n/2+1, n-n/2-1);
    root->update_rank();
    return root;
}


template<class K, class T>
Array<typename AVLTree<K,T>::Node> AVLTree<K, T>::to_array() const {
    Array<AVLTree<K,T>::Node> arr(tree_size);
    int i=0;
    for(auto it=begin(); it!=end(); ++it){
        auto node=it.node;
        arr[i]= *node;
        arr[i].left= nullptr;
        arr[i].right= nullptr;
        ++i;
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

template<class K, class T>
typename AVLTree<K,T>::Iterator AVLTree<K, T>::find_Kth_element(int k) const {
    if(k>size() || k<=0)
        return end();
    Node *n=root;
    Stack<Node*> s;
    int rank=n->left->rank.count+1;
    while(true){
        if(k== rank)
            return Iterator(n, s);
        else if(k>rank) {
            s.push(n);
            n = n->right;
            int left_rank= n->left ? n->left->rank.count : 0;
            rank += left_rank + 1;
        }
        else{
            s.push(n);
            n = n->left;
            int right_rank= n->right ? n->right->rank.count : 0;
            rank-= (1 + right_rank);
        }

    }


}

template<class K, class T>
void AVLTree<K, T>::erase(const K &key) {
    erase(find(key));
}


template<class K, class T>
int AVLTree<K, T>::get_sum_less_then(const AVLTree::Iterator&it) {
    if(it.node== nullptr)
        throw 0;//Iterator::InvalidIterator();
    auto to_find = it.node;
    auto n = root;
    unsigned int sum=n->left->rank.sum;
    while(true){
        if(n==to_find)
            return sum+n->data;
        else if(cmp(n->key, to_find->key)) {
            sum += n->data;
            n = n->right;
            int left_sum= n->left ? n->left->rank.sum : 0;
            sum += left_sum;
        }
        else{
            n = n->left;
            int right_sum= n->right ? n->right->rank.sum : 0;
            sum-= (right_sum+n->data);
        }

    }
}



#endif //AVLTREE_H
