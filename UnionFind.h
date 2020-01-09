#ifndef DATA_STRUCTURES_WET2_2_UNIONFIND_H
#define DATA_STRUCTURES_WET2_2_UNIONFIND_H
#include <iostream>
#include "Array.h"

using namespace std;
typedef unsigned int Key;

template <class T>
class UnionFind{
    struct Element;
    Array<Element> elements;
    Array<Key> parent;
    Key findRoot(Key i) const;
public:
    explicit UnionFind(int n);
    void Union(Key p,Key q);
    const T& find(Key i) const;
    T& get(Key);
    class InvalidInput : public std::exception{};
};

template <class T>
struct UnionFind<T>::Element{
    T data;
    size_t size;
};

template <class T>
UnionFind<T>::UnionFind(int n):
        elements(Array<Element>(n+1)), parent(Array<Key>(n+1)){
        if(n <= 0) throw InvalidInput();
        for(int i=1;i<=n;i++){
            elements[i].size = 1;
            parent[i]=i;
        }
}

template <class T>
void UnionFind<T>::Union(Key p,Key q) {
    if (((p<=0)||(p>elements.size()-1))|| ((q<=0)||(q>elements.size()-1)))
        throw InvalidInput();
    if(p==q)
        return;
    Key min,max;
    if(elements[findRoot(p)].size > elements[findRoot(q)].size){
        max = findRoot(p);
        min = q;
    }
    else{
        max = findRoot(q);
        min = p;
    }
    //printf("max = %d, min = %d\n",elements[max].size,elements[min].size);
    parent[min] = max;
    elements[max].data = T(elements[max].data,elements[min].data);
    elements[min].size = 0;
    elements[max].size++;
    //printArrs();
}

template<class T>
T &UnionFind<T>::get(Key i) {
    if(i <=0 || i>elements.size()-1)
        throw InvalidInput();
    return elements[findRoot(i)].data;
}


template <class T>
const T& UnionFind<T>::find(Key i) const{
    return get(i);
}

template <class T>
Key UnionFind<T>::findRoot(Key i) const{
    while(elements[i].size == 0){
        i = parent[i];
    }
    return i;
}

/*
template <class K>
void UnionFind<K>::printArrs() {
    for(Key i=0;i<elements.size();i++){
        printf("elements[%d].size = %d\n", i, elements[i].size);
    }
    for(Key i=0;i<elements.size();i++){
        printf("parent[%d] = %d\n",i,parent[i]);
    }
}

*/

#endif //DATA_STRUCTURES_WET2_2_UNIONFIND_H
