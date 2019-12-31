#ifndef DATA_STRUCTURES_WET2_HASHTABLE_H
#define DATA_STRUCTURES_WET2_HASHTABLE_H

#include <iostream>
#include "Array.h"
#include "List.h"
#include "utilities.h"


class HashFunction{
    unsigned int m;
public:
    explicit HashFunction(unsigned int m) : m(m){}
    unsigned int operator()(unsigned int n) const { return n%m;}
};

template <typename Key, typename T>
class HashTable {
public://TODO: remove after test
    struct Element{
        Key key;
        T data;
        explicit Element(Key, T=T());
    };
    Array<List<Element>> table;
    HashFunction hash;
    unsigned int r_max;
    int elements_count;
    void resize_table();
public:
    explicit HashTable(int r_max=3, unsigned int size=11);
    ~HashTable()= default;
    HashTable(const HashTable& ht)= default;
    HashTable(const HashTable& ht1, const HashTable& ht2);
    HashTable& operator=(const HashTable&)= default;
    const T& find(const Key&) const;
    bool contains(const Key&) const;
    T& operator[](const Key&);
    void insert(const Key&, const T& = T());
    void erase(const Key&);
    int size() const;
    class KeyNotExists : public std::exception{};
    class KeyAlreadyExists : public std::exception{};


};

template <typename Key, typename T>
HashTable<Key,T>::Element::Element(Key key, T data):key(key), data(data){}

template <typename Key, typename T>
HashTable<Key,T>::HashTable(int r_max, unsigned int size) :
    table(Array<List<Element>>(size)),
    hash(HashFunction(size)),
    r_max(r_max),
    elements_count(0)
    {}

template <typename Key, typename T>
HashTable<Key,T>::HashTable(const HashTable &ht1, const HashTable &ht2) :
        table(Array<List<Element>>(ht1.table.size()+ht2.table.size())),
        hash(HashFunction(table.size())),
        r_max(ht1.r_max),
        elements_count(ht1.elements_count+ht2.elements_count) {
    for (unsigned int i = 0; i < ht1.table.size(); ++i) {
        auto &l = ht1.table[i];
        for (auto &j : l)
            insert(j.key, j.data);
    }
    for (unsigned int i = 0; i < ht2.table.size(); ++i) {
        auto &l = ht2.table[i];
        for (auto &j : l)
            insert(j.key, j.data);
    }
}

template <typename Key, typename T>
const T &HashTable<Key,T>::find(const Key& key) const {
    const List<Element> &l = table[hash(key)];
    for(const Element &val : l) {
        if (val.key == key)
            return val.data;
    }
    throw KeyNotExists();
}

template <typename Key, typename T>
bool HashTable<Key,T>::contains(const Key& key) const {
    try{
        find(key);
        return true;
    }
    catch(KeyNotExists&){
        return false;
    }
}

template <typename Key, typename T>
T &HashTable<Key,T>::operator[](const Key& key) {
    List<Element> &l = table[hash(key)];
    for(Element &val : l) {
        if (val.key == key)
            return val.data;
    }
    if(table.size()/++elements_count<r_max)
        resize_table();
    auto it= l.push_back(Element(key));
    return (*it).data;
}

template <typename Key, typename T>
void HashTable<Key,T>::insert(const Key &key, const T &data) {
    if(contains(key))
        throw  KeyAlreadyExists();
    if(table.size()/++elements_count<r_max)
        resize_table();
    List<Element> &l = table[hash(key)];
    l.push_back(Element(key, data));


}


template <typename Key, typename T>
void HashTable<Key,T>::resize_table() {
    unsigned int new_size = first_prime_bigger(table.size()*2);
    HashTable new_ht(r_max, new_size);
    for(unsigned int i=0;i<table.size();++i){
        auto list = table[i];
        for(auto& element : list){
            new_ht.insert(element.key, element.data);
        }
    }
    *this=new_ht;
}


template <typename Key, typename T>
void HashTable<Key,T>::erase(const Key &key) {
    if(!contains(key))
        throw  KeyNotExists();
    List<Element> &l = table[hash(key)];
    for(auto i=l.begin(); i!= l.end();) {
        if ((*i).key == key) {
            l.erase(i);
            break;
        }
    }
}

template<typename Key, typename T>
int HashTable<Key, T>::size() const {
    return elements_count;
}



#endif //DATA_STRUCTURES_WET2_HASHTABLE_H
