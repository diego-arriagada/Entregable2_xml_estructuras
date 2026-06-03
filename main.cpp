#include <iostream>
#include "tree.hpp"

using namespace std;

int main() {
    Tree tree(3);

    tree.insert(0, 1);
    tree.insert(1, 2);
    tree.insert(1, 3);
    tree.insert(1, 4);

    tree.insert(2, 5);
    tree.insert(2, 6);

    cout << "Tamaño: " << tree.size() << endl;

    cout << "PreOrder: ";
    for (int x : tree.preOrder())
        cout << x << " ";
    cout << endl;

    cout << "InOrder: ";
    for (int x : tree.inOrder())
        cout << x << " ";
    cout << endl;

    cout << "PostOrder: ";
    for (int x : tree.postOrder())
        cout << x << " ";
    cout << endl;

    cout << "Padre de 5: " << tree.parent(5) << endl;

    cout << "Hijo de 1: ";
    for (int x : tree.children(1))
        cout << x << " ";
    cout << endl;

    tree.remove(2);

    cout << "PreOrder después de remove(2): ";
    for (int x : tree.preOrder())
        cout << x << " ";
    cout << endl;

    return 0;
}