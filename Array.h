#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

//#include "SparseMatrix.h"

template<class T>
class Array {
private:
    unsigned int array_size;
    T* data;
public:
    unsigned int size() const;
    explicit Array(unsigned int size);
    ~Array();
    Array(const Array<T>&);
    Array<T>& operator=(const Array&);
    void resize(unsigned int n);
    const T* const operator*() const;
    T& operator[](unsigned int index);
    const T& operator[](unsigned int index) const;
    class InvalidIndex : public std::exception{};

};

template<class T>
Array<T>::Array(unsigned int size): array_size(size), data(new T[size]){}

template<class T>
Array<T>::Array(const Array<T>& array): array_size(array.array_size), data(new T[array_size]) {
    for(unsigned int i=0; i < array_size; ++i){
        data[i]= array[i];
    }
}



template<class T>
Array<T>::~Array() {
    delete[] data;
}


template<class T>
Array<T> &Array<T>::operator=(const Array& array) {
    if (&array==this)
        return *this;
    array_size=array.size();
    delete[] data;
    data= new T[array_size];
    for(unsigned int i=0; i<array_size; ++i){
        data[i]= array[i];
    }
    return *this;
}

template<class T>
T &Array<T>::operator[](unsigned int index) {
    if(index<0 || index>=array_size)
        throw InvalidIndex();
    return data[index];
}

template<class T>
const T &Array<T>::operator[](unsigned int index) const {
    if(index<0 || index>=array_size)
        throw InvalidIndex();
    return data[index];
}

template<class T>
unsigned int Array<T>::size() const {
    return array_size;
}

template<class T>
const T *const Array<T>::operator*() const{
    return data;
}

template<class T>
void Array<T>::resize(unsigned int n) {
    Array<T> new_arr = Array(n);
    for(unsigned int i=0; i<n && i<array_size; ++i)
        new_arr[i]= (*this)[i];
    *this=new_arr;
}


#endif //ARRAY_H
