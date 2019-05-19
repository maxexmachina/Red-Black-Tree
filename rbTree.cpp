//
// Created by Maxim Morgunov on 15.05.2019.
//
#include <iostream>
#include "rbTree.h"

#define RED 1
#define BLACK 0

template<typename T>
void cleanUp(struct tree<T>* root) {
    if(root->left != nullptr)
        cleanUp(root->left);
    if(root->right != nullptr)
        cleanUp(root->right);
    delete root;
}

    template<typename T>
    struct tree<T>* redBlack<T>::create(T value) {
        struct tree<T>* temp = new struct tree<T>;
        temp->a = value;
        temp->left = nullptr;
        temp->right = nullptr;
        temp->parent = nullptr;
        temp->colour = RED;
        return temp;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::grandpa(const struct tree<T>* root) const {
        if (root and root->parent)
            return root->parent->parent;
        else
            return nullptr;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::uncle(const struct tree<T>* root) const {
        struct tree<T>* g = grandpa(root);
        if (g == nullptr)
            return nullptr;
        if (root->parent == g->left)
            return g->right;
        else
            return g->left;

    }

    template<typename T>
    struct tree<T>* redBlack<T>::sibling(const struct tree<T>* root) const {
        if (root == root->parent->left)
            return root->parent->right;
        else
            return root->parent->left;
    }

    template<typename T>
    void redBlack<T>::rot_left(struct tree<T>* root) const {
        struct tree<T>* pivot = root->right;

        pivot->parent = root->parent;
        if (root->parent != nullptr) {
            if (root->parent->left == root)
                root->parent->left = pivot;
            else
                root->parent->right = pivot;
        }

        root->right = pivot->left;
        if (pivot->left != nullptr)
            pivot->left->parent = root;

        root->parent = pivot;
        pivot->left = root;
    }

    template<typename T>
    void redBlack<T>::rot_right(struct tree<T>* root) const {
        struct tree<T>* pivot = root->left;

        pivot->parent = root->parent;
        if (root->parent != nullptr) {
            if (root->parent->right == root)
                root->parent->right = pivot;
            else
                root->parent->left = pivot;
        }

        root->left = pivot->right;
        if (pivot->right != nullptr)
            pivot->right->parent = root;

        root->parent = pivot;
        pivot->right = root;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::locate(struct tree<T>* root, T value) {

        struct tree<T>* tmp = create(value);

        if (root == nullptr) {
            delete tmp;
            return nullptr;
        }
        else {
            if (value == root->a) {
                tmp = root;
                return tmp;
            }
            if(value < root->a) {
                if(root->left == nullptr) {
                    delete tmp;
                    return nullptr;
                }
                else {
                    tmp = locate(root->left, value);
                    return tmp;
                }
            }
            else if(value > root->a) {
                if(root->right == nullptr) {
                    delete tmp;
                    return nullptr;
                }
                else {
                    tmp = locate(root->right, value);
                    return tmp;
                }
            }
            else {
                delete tmp;
                return nullptr;
            }
        }
    }

    template<typename T>
    void redBlack<T>::replace_child(struct tree<T>* root, struct tree<T>* child) const {
        struct tree<T>* s = sibling(child);
        if (s != nullptr) {
            s->parent = child;
            if (s == root->left)
                child->left = s;
            else
                child->right = s;
        }
        child->parent = root->parent;
        if (root->parent != nullptr) {
            if (root == root->parent->left)
                root->parent->left = child;
            else
                root->parent->right = child;
        }
    }

    template<typename T>
    void redBlack<T>::delete_case6(struct tree<T>* root) const {
        struct tree<T>* s = sibling(root);

        s->colour = root->parent->colour;
        root->parent->colour = BLACK;
        if (root == root->parent->left) {
            s->right->colour = BLACK;
            rot_left(root->parent);
        } else {
            s->left->colour = BLACK;
            rot_right(root->parent);
        }
    }

    template<typename T>
    void redBlack<T>::delete_case5(struct tree<T>* root) const {
        struct tree<T>* s = sibling(root);

        if (s->colour == BLACK) {
            if ((root == root->parent->left) and (s->right->colour == BLACK) and (s->left->colour == RED)) {
                s->colour = RED;
                s->left->colour = BLACK;
                rot_right(s);
            } else if ((root = root->parent->right) and (s->left->colour == BLACK) and (s->right->colour == RED)) {
                s->colour = RED;
                s->right->colour = BLACK;
                rot_left(s);
            }
        }
        delete_case6(root);
    }

    template<typename T>
    struct tree<T>* redBlack<T>::delete_case4(struct tree<T>* root) const {
        struct tree<T>* s = sibling(root);

        if ((root->parent->colour == RED) and (s->colour == BLACK) and (s->left->colour == BLACK) and (s->right->colour == BLACK)) {
            s->colour = RED;
            root->parent->colour = BLACK;
        } else
            delete_case5(root);
    }

    template<typename T>
    void redBlack<T>::delete_case3(struct tree<T>* root) const {
        struct tree<T>* s = sibling(root);

        if ((root->parent->colour == BLACK) and (s->colour == BLACK) and (s->left->colour == BLACK) and (s->right->colour = BLACK)) {
            s->colour = RED;
            delete_case1(root->parent);
        } else
            delete_case4(root);
    }

    template<typename T>
    void redBlack<T>::delete_case2(struct tree<T>* root) const {
        struct tree<T>* s = sibling(root);

        if(s->colour == RED) {
            root->parent->colour = RED;
            s->colour = BLACK;
            if (root == root->parent->left)
                rot_left(root->parent);
            else
                rot_right(root->parent);
        }
        delete_case3(root);
    }

    template<typename T>
    void redBlack<T>::delete_case1(struct tree<T>* root) const {
        if (root->parent != nullptr)
            delete_case2(root);
    }

    template<typename T>
    struct tree<T>* redBlack<T>::delete_node(struct tree<T>* root) {
        if (root->left != nullptr or root->right != nullptr) {
            struct tree<T>* child = (root->right == nullptr) ? root->left : root->right;

            replace_child(root, child);
            if (root->colour == BLACK) {
                if (child->colour == RED)
                    child->colour = BLACK;
                else
                    delete_case1(child);
            }
            struct tree<T>* tmp = child;
            while (tmp->parent != nullptr) {
                tmp = tmp->parent;
            }
            return tmp;
        } else {
            if (root == root->parent->right)
                root->parent->right = nullptr;
            else
                root->parent->left = nullptr;
            delete(root);
            struct tree<T>* tmp = root->parent;
            while (tmp->parent != nullptr) {
                tmp = tmp->parent;
            }
            return tmp;
        }
    }

    template<typename T>
    struct tree<T>* redBlack<T>::insert_case5(struct tree<T>* root) const {
        struct tree<T>* g = grandpa(root);

        root->parent->colour = BLACK;
        g->colour = RED;
        if ((root == root->parent->left) and (root->parent == g->left)) {
            rot_right(g);
        } else {
            rot_left(g);
        }
        struct tree<T>* tmp = root;
        while (tmp->parent != nullptr) {
            tmp = tmp->parent;
        }
        return tmp;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::insert_case4(struct tree<T>* root) const {
        struct tree<T>* g = grandpa(root);

        if ((root == root->parent->right and root->parent == g->left)) {
            rot_left(root->parent);
            root = root->left;
        } else if ((root == root->parent->left) and (root->parent == g->right)) {
            rot_right(root->parent);
            root = root->right;
        }
        insert_case5(root);
        struct tree<T>* tmp = root;
        while (tmp->parent != nullptr) {
            tmp = tmp->parent;
        }
        return tmp;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::insert_case3(struct tree<T>* root) const {
        struct tree<T>* u = uncle(root),  *g;

        if ((u != nullptr) and (u->colour == RED)) {
            root->parent->colour = BLACK;
            u->colour = BLACK;
            g = grandpa(root);
            g->colour = RED;
            insert_case1(g);
        } else {
            insert_case4(root);
        }
        struct tree<T>* tmp = root;
        while (tmp->parent != nullptr) {
            tmp = tmp->parent;
        }
        return tmp;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::insert_case2(struct tree<T>* root) const {
        struct tree<T>* tmp = root;
        if (root->parent->colour == BLACK) {
            while (tmp->parent != nullptr) {
                tmp = tmp->parent;
            }
            return tmp;
        }
        else
            insert_case3(root);
        while (tmp->parent != nullptr) {
            tmp = tmp->parent;
        }
        return tmp;
    }

    template<typename T>
    struct tree<T>* redBlack<T>::insert_case1(struct tree<T>* root) const {
        struct tree<T>* tmp = root;
        if (root->parent == nullptr) {
            root->colour = BLACK;
            return root;
        }
        else
            insert_case2(root);
        while (tmp->parent != nullptr) {
            tmp = tmp->parent;
        }
        return tmp;

    }

    template<typename T>
    struct tree<T>* redBlack<T>::inserter(struct tree<T>* root, T value) {

        struct tree<T>* tmp = create(value);

        if (root == nullptr) {
            return tmp;
        }
        else {
            if(value < root->a) {
                if(root->left == nullptr) {
                    root->left = tmp;
                    tmp->parent = root;
                    return tmp;
                }
                else {
                    tmp = inserter(root->left, value);
                    return tmp;
                }
            }
            else if(value >= root->a) {
                if(root->right == nullptr) {
                    root->right = tmp;
                    tmp->parent = root;
                    return tmp;
                }
                else {
                    tmp = inserter(root->right, value);
                    return tmp;
                }
            }
            else {
                delete tmp;
                return nullptr;
            }
        }
    }

    template<typename T>
    void redBlack<T>::printerFull(const struct tree<T>* root) const {
        if(root != nullptr) {
            std::cout << "Value: " << root->a << "(" << root->colour << "), ";
            std::cout << "Left: ";
            printerFull(root->left);
            std::cout << ", " << "Right: ";
            printerFull(root->right);
        }
    }

    template<typename T>
    void redBlack<T>::printer(const struct tree<T>* root) const {
        if (root) {
            std::cout << root->a << ", ";
            printer(root->left);
            printer(root->right);
        }
    }

    template<typename T>
    void redBlack<T>::insert(T value) {
        mainRoot = insert_case1(inserter(mainRoot, value));
    }

    template<typename T>
    bool redBlack<T>::exists(T value) {
        if(locate(mainRoot, value) != nullptr)
            return true;
        else
            return false;
    }

    template<typename T>
    void redBlack<T>::remove(T value) {
        if(exists(value))
            mainRoot = delete_node(locate(mainRoot, value));
    }

    template<typename T>
    void redBlack<T>::printSome() {
        printer(mainRoot);
    }

    template<typename T>
    void redBlack<T>::print() {
        printerFull(mainRoot);
    }

    template<typename T>
    redBlack<T>::~redBlack() {
        cleanUp(mainRoot);
        std::cout << "Destructor called" << std::endl;
    }


template class redBlack<float>;
template class redBlack<int>;
template class redBlack<std::string>;