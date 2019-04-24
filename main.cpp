#include <iostream>

#define RED 1
#define BLACK 0

struct tree {
    int a;
    int colour;
    struct tree* left;
    struct tree* right;
    struct tree* parent;
};

struct tree* insert_case1(struct tree* root);

struct tree* create(int value) {
    struct tree* temp = new struct tree;
    temp->a = value;
    temp->left = nullptr;
    temp->right = nullptr;
    temp->parent = nullptr;
    temp->colour = RED;
    return temp;
}

void delete_case2(struct tree* root);

struct tree* grandpa(struct tree* root) {
    if ((root != nullptr) and (root->parent != nullptr) and (root->parent->parent != nullptr))
        return root->parent->parent;
    else
        return nullptr;
}

struct tree* uncle(struct tree* root) {
    struct tree* g = grandpa(root);
    if (g == nullptr)
        return nullptr;
    if (root->parent == g->left)
        return g->right;
    else
        return g->left;

}

struct tree* sibling(struct tree* root) {
    if (root == root->parent->left)
        return root->parent->right;
    else
        return root->parent->left;
}

void rot_left(struct tree* root) {
    struct tree* pivot = root->right;

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

void rot_right(struct tree* root) {
    struct tree* pivot = root->left;

    pivot->parent = root->parent;
    if (root->parent->right == root)
        root->parent->right = pivot;
    else
        root->parent->left = pivot;

    root->left = pivot->right;
    if (pivot->right != nullptr)
        pivot->right->parent = root;

    root->parent = pivot;
    pivot->right = root;
}

struct tree* locate(struct tree* root, int value) {

    struct tree *tmp = create(value);

    if (root == nullptr) {
        return nullptr;
    }
    else {
        if (value == root->a) {
            tmp = root;
            return tmp;
        }
        if(value < root->a) {
            if(root->left == nullptr)
                return nullptr;
            else {
                tmp = locate(root->left, value);
                return tmp;
            }
        }
        else if(value > root->a) {
            if(root->right == nullptr)
                return nullptr;
            else {
                tmp = locate(root->right, value);
                return tmp;
            }
        }
        else return nullptr;
    }
}

void replace_child(struct tree* root, struct tree* child) {
    struct tree* s = sibling(child);
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

void delete_case6(struct tree* root) {
    struct tree* s = sibling(root);

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

void delete_case5(struct tree* root) {
    struct tree* s = sibling(root);

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

struct tree* delete_case4(struct tree* root) {
    struct tree* s = sibling(root);

    if ((root->parent->colour == RED) and (s->colour == BLACK) and (s->left->colour == BLACK) and (s->right->colour == BLACK)) {
        s->colour = RED;
        root->parent->colour = BLACK;
    } else
        delete_case5(root);
}

void delete_case1(struct tree* root) {
    if (root->parent != nullptr)
        delete_case2(root);
}

void delete_case3(struct tree* root) {
    struct tree* s = sibling(root);

    if ((root->parent->colour == BLACK) and (s->colour == BLACK) and (s->left->colour == BLACK) and (s->right->colour = BLACK)) {
        s->colour = RED;
        delete_case1(root->parent);
    } else
        delete_case4(root);
}

void delete_case2(struct tree* root) {
    struct tree* s = sibling(root);

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

struct tree* delete_node(struct tree* root) {
        if (root->left != nullptr or root->right != nullptr) {
            struct tree *child = (root->right == nullptr) ? root->left : root->right;

            replace_child(root, child);
            if (root->colour == BLACK) {
                if (child->colour == RED)
                    child->colour = BLACK;
                else
                    delete_case1(child);
            }
            struct tree* tmp = child;
            while (tmp->parent != nullptr) {
                tmp = tmp->parent;
            }
            //free(root);
            return tmp;
        } else {
           // delete_case1(root);
            if (root == root->parent->right)
                root->parent->right = nullptr;
            else
                root->parent->left = nullptr;
            free(root);
            struct tree* tmp = root->parent;
            while (tmp->parent != nullptr) {
                tmp = tmp->parent;
            }
            //free(root);
            return tmp;
        }
}

struct tree* insert_case5(struct tree* root) {
    struct tree* g = grandpa(root);

    root->parent->colour = BLACK;
    g->colour = RED;
    if ((root == root->parent->left) and (root->parent == g->left)) {
        rot_right(g);
    } else {
        rot_left(g);
    }
    struct tree* tmp = root;
    while (tmp->parent != nullptr) {
        tmp = tmp->parent;
    }
    return tmp;
}

struct tree* insert_case4(struct tree* root) {
    struct tree* g = grandpa(root);

    if ((root == root->parent->right and root->parent == g->left)) {
        rot_left(root->parent);
        root = root->left;
    } else if ((root == root->parent->left) and (root->parent = g->right)) {
        rot_right(root->parent);
        root = root->right;
    }
    insert_case5(root);
    struct tree* tmp = root;
    while (tmp->parent != nullptr) {
        tmp = tmp->parent;
    }
    return tmp;
}

struct tree* insert_case3(struct tree* root) {
    struct tree* u = uncle(root),  *g;

    if ((u != nullptr) and (u->colour == RED)) {
        root->parent->colour = BLACK;
        u->colour = BLACK;
        g = grandpa(root);
        g->colour = RED;
        insert_case1(g);
    } else {
        insert_case4(root);
    }
    struct tree* tmp = root;
    while (tmp->parent != nullptr) {
        tmp = tmp->parent;
    }
    return tmp;
}

struct tree* insert_case2(struct tree* root) {
    struct tree* tmp = root;
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

struct tree* insert_case1(struct tree* root) {
    struct tree* tmp = root;
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

struct tree* insert(struct tree* root, int value) {

    struct tree *tmp = create(value);

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
                tmp = insert(root->left, value);
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
                tmp = insert(root->right, value);
                return tmp;
            }
        }
        else return nullptr;
    }
    }

void print(struct tree* root) {
    if(root != nullptr) {
        /*if (root->parent != 0) {
            printf("%d\n", root->parent->a);
        }*/
        std::cout << root->a << ", ";
        print(root->left);
        print(root->right);
    }

}

int main(int argc, char* argv[]) {
    int N;
    std::cin >> N;
    int* array = new int[N];
    for (int i = 0; i < N; i++) {
        std::cin >> array[i];
    }
    struct tree* root = create(array[0]);
    insert_case1(root);
    //struct tree** grove = malloc((N-1)* sizeof(struct tree));
    for (int i = 1; i < N; i++) {
        root = insert_case1(insert(root, array[i]));
        print(root);
        std::cout << std::endl;
    }

    //print(root);
    //root = delete_node(locate(root, 3));
    //print(root);
    //printf("%d", locate(root, 1)->a);
    //free(root);
    delete[] array;

    return 0;
}

