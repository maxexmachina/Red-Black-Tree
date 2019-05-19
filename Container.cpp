//
// Created by Maxim Morgunov on 18.05.2019.
//

#include <iostream>
#include "Container.h"

template<typename T>
void Container<T>::insert(T value) {}

template<typename T>
bool Container<T>::exists(T value) {}

template<typename T>
void Container<T>::remove(T value) {}

template<typename T>
void Container<T>::print() {}

template<typename T>
Container<T>::~Container() {}

template class Container<float>;
template class Container<int>;
template class Container<std::string>;