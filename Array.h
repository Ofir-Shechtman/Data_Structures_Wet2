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
    Array(const Array<T>& arr1, const Array<T>& arr2);
    Array<T>& operator=(const Array&);
    const T* const operator*();
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
Array<T>::Array(const Array<T>& arr1, const Array<T>& arr2) :
        array_size(arr1.size()+arr2.size())
{
    unsigned int n1=arr1.size(), n2=arr2.size();
    Array<T>& arr3 = *this;
    int i = 0, j = 0, k = 0;
    while (i<n1 && j <n2)
    {
        if (arr1[i] < arr2[j])
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
}

template<class T>
Array<T>::~Array() {
    delete[] data;
}


template<class T>
Array<T> &Array<T>::operator=(const Array& array) {
    if (&array==this)
        return *this;
    delete data;
    array_size=array.size();
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
const T *const Array<T>::operator*() {
    return data;
}


#endif //ARRAY_H
