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

class Container {
public:
    virtual void insert(int value) = 0;
    virtual bool exists(int value) = 0;
    virtual void remove(int value) = 0;
    virtual void print() { }

    virtual ~Container() = default;
};

class redBlack : public Container {
private:
    struct tree* mainRoot = nullptr;

    struct tree* create(int value) {
        struct tree* temp = new struct tree;
        temp->a = value;
        temp->left = nullptr;
        temp->right = nullptr;
        temp->parent = nullptr;
        temp->colour = RED;
        return temp;
    }

    struct tree* grandpa(const struct tree* root) const {
        if (root and root->parent)
            return root->parent->parent;
        else
            return nullptr;
    }

    struct tree* uncle(const struct tree* root) const {
        struct tree* g = grandpa(root);
        if (g == nullptr)
            return nullptr;
        if (root->parent == g->left)
            return g->right;
        else
            return g->left;

    }

    struct tree* sibling(const struct tree* root) const {
        if (root == root->parent->left)
            return root->parent->right;
        else
            return root->parent->left;
    }

    void rot_left(struct tree* root) const {
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

    void rot_right(struct tree* root) const {
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

    void replace_child(struct tree* root, struct tree* child) const {
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

    void delete_case6(struct tree* root) const {
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

    void delete_case5(struct tree* root) const {
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

    struct tree* delete_case4(struct tree* root) const {
        struct tree* s = sibling(root);

        if ((root->parent->colour == RED) and (s->colour == BLACK) and (s->left->colour == BLACK) and (s->right->colour == BLACK)) {
            s->colour = RED;
            root->parent->colour = BLACK;
        } else
            delete_case5(root);
    }

    void delete_case3(struct tree* root) const {
        struct tree* s = sibling(root);

        if ((root->parent->colour == BLACK) and (s->colour == BLACK) and (s->left->colour == BLACK) and (s->right->colour = BLACK)) {
            s->colour = RED;
            delete_case1(root->parent);
        } else
            delete_case4(root);
    }

    void delete_case2(struct tree* root) const {
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

    void delete_case1(struct tree* root) const {
        if (root->parent != nullptr)
            delete_case2(root);
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
            return tmp;
        } else {
            if (root == root->parent->right)
                root->parent->right = nullptr;
            else
                root->parent->left = nullptr;
            delete(root);
            struct tree* tmp = root->parent;
            while (tmp->parent != nullptr) {
                tmp = tmp->parent;
            }
            return tmp;
        }
    }

    struct tree* insert_case5(struct tree* root) const {
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

    struct tree* insert_case4(struct tree* root) const {
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

    struct tree* insert_case3(struct tree* root) const {
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

    struct tree* insert_case2(struct tree* root) const {
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

    struct tree* insert_case1(struct tree* root) const {
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

    struct tree* inserter(struct tree* root, int value) {

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
            else return nullptr;
        }
    }

    void printerFull(const struct tree* root) const {
        if(root != nullptr) {
            std::cout << "Value: " << root->a << "(" << root->colour << "), ";
            std::cout << "Left: ";
            printerFull(root->left);
            std::cout << ", " << "Right: ";
            printerFull(root->right);
        }
    }

    void printer(const struct tree* root) const{
        if (root) {
            std::cout << root->a << ", ";
            printer(root->left);
            printer(root->right);
        }
    }

public:
    void insert(int value) override {
            mainRoot = insert_case1(inserter(mainRoot, value));
    }

    bool exists(int value) override {
        if(locate(mainRoot, value))
            return true;
        else
            return false;
    }

    void remove(int value) override {
        if(exists(value))
        mainRoot = delete_node(locate(mainRoot, value));
    }

    void printSome() {
            printer(mainRoot);
        }

    void print() override {
        printerFull(mainRoot);
        }

    ~redBlack() override {
        std::cout << "Destructor called" << std::endl;
    }
};


int main() {

    int N;
    std::cin >> N;
    int* array = new int[N];
    for (int i = 0; i < N; i++) {
        std::cin >> array[i];
    }
    Container* rb = new redBlack;
    for (int i = 0; i < N; i++) {
        rb->insert(array[i]);
        rb->print();
        std::cout << std::endl;
    }
    /*Container* rb = new redBlack;
    for(int i = 1; i < 10; i++)
        rb->insert(i*i);
    rb->print();

    if(rb->exists(25))
        std::cout << "The value of 25 has been found" << std::endl;

    rb->remove(25);*/

    //std::cout << std::endl;
    //rb.remove(3);
    //rb->print();
    std::cout << std::endl;

    //std::cout << root->left->parent->a << std::endl;

    //delete[] array;
    delete rb;
    return 0;
}

