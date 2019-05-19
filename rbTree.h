//
// Created by Maxim Morgunov on 15.05.2019.
//

#ifndef OBJECTTREE_RBTREE_H
#define OBJECTTREE_RBTREE_H

#include <iostream>
#include <string>
#include "Container.h"

template<typename T>
struct tree {
    T a;
    int colour;
    struct tree* left;
    struct tree* right;
    struct tree* parent;
};

template<typename T>
void cleanUp(struct tree<T>* root);

template<typename T>
class redBlack : public Container<T> {
private:
    struct tree<T> *mainRoot = nullptr;

    struct tree<T> *create(T value);

    struct tree<T> *grandpa(const struct tree<T> *root) const;

    struct tree<T> *uncle(const struct tree<T> *root) const;

    struct tree<T> *sibling(const struct tree<T> *root) const;

    void rot_left(struct tree<T> *root) const;

    void rot_right(struct tree<T> *root) const;

    struct tree<T> *locate(struct tree<T> *root, T value);

    void replace_child(struct tree<T> *root, struct tree<T> *child) const;

    void delete_case6(struct tree<T> *root) const;

    void delete_case5(struct tree<T> *root) const;

    struct tree<T> *delete_case4(struct tree<T> *root) const;

    void delete_case3(struct tree<T> *root) const;

    void delete_case2(struct tree<T> *root) const;

    void delete_case1(struct tree<T> *root) const;

    struct tree<T> *delete_node(struct tree<T> *root);

    struct tree<T> *insert_case5(struct tree<T> *root) const;

    struct tree<T> *insert_case4(struct tree<T> *root) const;

    struct tree<T> *insert_case3(struct tree<T> *root) const;

    struct tree<T> *insert_case2(struct tree<T> *root) const;

    struct tree<T> *insert_case1(struct tree<T> *root) const;

    struct tree<T> *inserter(struct tree<T> *root, T value);

    void printerFull(const struct tree<T> *root) const;

    void printer(const struct tree<T> *root) const;

public:
    void insert(T value) override;

    bool exists(T value) override;

    void remove(T value) override;

    void printSome();

    void print() override;

    ~redBlack() override;
};



#endif //OBJECTTREE_RBTREE_H
