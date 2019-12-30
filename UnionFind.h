#ifndef DATA_STRUCTURES_WET2_2_UNIONFIND_H
#define DATA_STRUCTURES_WET2_2_UNIONFIND_H
#include <iostream>
#include "Array.h"

using namespace std;
typedef int K;

class UnionFind{
    struct element;
    Array<element> elements;
    Array<int> parent;
    int array_size;
    int findRoot(int i);
public:
    void printArrs();
    explicit UnionFind(int n);
    void Union(int p,int q);
    int Find(int i);
};

struct UnionFind::element{
    K data;
    unsigned int size;
    //TODO: constructor?
};

UnionFind::UnionFind(int n):
        elements(Array<element>(n)), parent(Array<int>(n)), array_size(n){
        for(int i=0;i<n;i++){
            elements[i].size = 1;
            parent[i]=i;
        }
}

void UnionFind::Union(int p,int q) {
    if ((p==q) || ((p<0)||(p>=array_size))|| ((q<0)||(q>=array_size)))return;
    int max = elements[findRoot(p)].size > elements[findRoot(q)].size ? p : q;
    int min = elements[findRoot(p)].size <= elements[findRoot(q)].size ? p : q;
    printf("max = %d, min = %d\n",max,min);
    parent[min] = max;
    elements[findRoot(max)].size++;
    elements[min].size = 0;
}

int UnionFind::Find(int i)  {
    return findRoot(i);
}

int UnionFind::findRoot(int i) {
    while(elements[i].size == 0){
        i = parent[i];
    }
    return i;
}

void UnionFind::printArrs() {
    for(int i=0;i<array_size;i++){
        printf("elements[%d].size = %d\n", i, elements[i].size);
    }
    for(int i=0;i<array_size;i++){
        printf("parent[%d] = %d\n",i,parent[i]);
    }
}

#endif //DATA_STRUCTURES_WET2_2_UNIONFIND_H
