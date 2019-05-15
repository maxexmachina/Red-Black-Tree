#include <iostream>
#include <string>
#include "rbTree.h"

int main() {

    int N;
    std::cin >> N;
    float* array = new float[N];
    for (int i = 0; i < N; i++) {
        std::cin >> array[i];
    }
    Container<float>* rb = new redBlack<float>;
    for (int i = 0; i < N; i++) {
        rb->insert(array[i]);
        rb->print();
        std::cout << std::endl;
    }

    std::cout << std::endl;
    rb->remove(23.5);
    rb->print();
    std::cout << std::endl;

    delete[] array;
    delete rb;
    return 0;
}

