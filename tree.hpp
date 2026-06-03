#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

struct XmlData {
    string tag;
    string text;
};

class Tree {
public:
    struct Node {
        XmlData data;
        Node* parent;
        vector<Node*> children;

        Node(string tag, string text = "", Node* p = nullptr);
    };

private:
    Node* rootNode;
    int treeSize;

    void deleteSubtree(Node* node);
    int countSubtree(Node* node);

    void listarRec(Node* node);
    void precursoresRec(Node* node);

    bool isMainBook(Node* node);
    bool shouldDeleteByRating(Node* book, double r);
    bool isPrecursor(Node* book);

    int toInt(string value);
    double toDouble(string value);

public:
    Tree();
    ~Tree();

    bool isEmpty();
    int size();

    Node* root();

    Node* insert(Node* parent, string tag, string text = "");

    Node* firstChild(Node* node, string tag);
    string childText(Node* node, string tag);

    void listar();
    void borrar_ratings(double r);
    void precursores();
};

#endif