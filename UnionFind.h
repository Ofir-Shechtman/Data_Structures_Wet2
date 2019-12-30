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
    size_t array_size;
    Key findRoot(Key i) const;
public:
    explicit UnionFind(int n);
    void Union(Key p,Key q);
    const T& Find(Key i) const;

    void printArrs();
};

template <class T>
struct UnionFind<T>::Element{
    T data;
    size_t size;
};

template <class T>
UnionFind<T>::UnionFind(int n):
        elements(Array<Element>(n+1)), parent(Array<Key>(n+1)), array_size(n+1){
        for(int i=1;i<=n;i++){
            elements[i].size = 1;
            parent[i]=i;
        }
}

template <class T>
void UnionFind<T>::Union(Key p,Key q) {
    Key min,max;
    if ((p==q) || ((p<=0)||(p>array_size))|| ((q<=0)||(q>array_size)))return;
    if(elements[findRoot(p)].size > elements[findRoot(q)].size){
        max = findRoot(p);
        min = q;
    }
    else{
        max = findRoot(q);
        min = p;
    }
    printf("max = %d, min = %d\n",elements[max].size,elements[min].size);
    parent[min] = max;
    //elements[max].data = T(max,min);
    elements[min].size = 0;
    elements[max].size++;
}

template <class T>
const T& UnionFind<T>::Find(Key i) const{
    return elements[findRoot(i)].data;
}

template <class T>
Key UnionFind<T>::findRoot(Key i) const{
    while(elements[i].size == 0){
i = parent[i];
}
return i;
}

template <class K>
void UnionFind<K>::printArrs() {
    for(Key i=0;i<array_size;i++){
        printf("elements[%d].size = %d\n", i, elements[i].size);
    }
    for(Key i=0;i<array_size;i++){
        printf("parent[%d] = %d\n",i,parent[i]);
    }
}

#endif //DATA_STRUCTURES_WET2_2_UNIONFIND_H
