//
// Created by Maxim Morgunov on 18.05.2019.
//

#ifndef OBJECTTREE_CONTAINER_H
#define OBJECTTREE_CONTAINER_H

template<typename T>
class Container {
public:
    virtual void insert(T value);

    virtual bool exists(T value);

    virtual void remove(T value);

    virtual void print();

    virtual ~Container();
};

#endif //OBJECTTREE_CONTAINER_H
