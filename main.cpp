#include <iostream>
#include "UnionFind.h"

using namespace std;

int main(){
    UnionFind<int> a(5);
    a.Union(0,1);
    a.Union(0,2);
    a.Union(0,3);
    a.Union(0,4);
    a.Union(0,5);

    a.printArrs();
    return 0;
}