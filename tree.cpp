#include "tree.hpp"
#include <algorithm>

Tree::Node::Node(string tag, string text, Node* p) {
    data.tag = tag;
    data.text = text;
    parent = p;
}

Tree::Tree() {
    rootNode = new Node("GoodReads");
    treeSize = 1;
}

Tree::~Tree() {
    deleteSubtree(rootNode);
}

bool Tree::isEmpty(){
    return treeSize == 0;
}

int Tree::size(){
    return treeSize;
}

Tree::Node* Tree::root(){
    return rootNode;
}

Tree::Node* Tree::insert(Node* parent, string tag, string text) {
    if (parent == nullptr) {
        return nullptr;
    }

    Node* newNode = new Node(tag, text, parent);
    parent->children.push_back(newNode);
    treeSize++;

    return newNode;
}

void Tree::deleteSubtree(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (Node* child : node->children) {
        deleteSubtree(child);
    }

    delete node;
}

int Tree::countSubtree(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    int count = 1;

    for (Node* child : node->children) {
        count += countSubtree(child);
    }

    return count;
}

Tree::Node* Tree::firstChild(Node* node, string tag){
    if (node == nullptr) {
        return nullptr;
    }

    for (Node* child : node->children) {
        if (child->data.tag == tag) {
            return child;
        }
    }

    return nullptr;
}

string Tree::childText(Node* node, string tag){
    Node* child = firstChild(node, tag);

    if (child == nullptr) {
        return "";
    }

    return child->data.text;
}

int Tree::toInt(string value){
    if (value == "") {
        return -1;
    }

    try {
        return stoi(value);
    } catch (...) {
        return -1;
    }
}

double Tree::toDouble(string value){
    if (value == "") {
        return -1.0;
    }

    try {
        return stod(value);
    } catch (...) {
        return -1.0;
    }
}

bool Tree::isMainBook(Node* node){
    return node != nullptr &&
           node->data.tag == "book" &&
           node->parent == rootNode;
}

void Tree::listar(){
    listarRec(rootNode);
}

void Tree::listarRec(Node* node){
    if (node == nullptr) {
        return;
    }

    if (isMainBook(node)) {
        cout << childText(node, "id") << endl;
    }

    for (Node* child : node->children) {
        listarRec(child);
    }
}

bool Tree::shouldDeleteByRating(Node* book, double r){
    if (!isMainBook(book)) {
        return false;
    }

    double rating = toDouble(childText(book, "average_rating"));

    if (rating < 0) {
        return false;
    }

    return rating <= r;
}

void Tree::borrar_ratings(double r) {
    vector<Node*>& books = rootNode->children;

    auto it = books.begin();

    while (it != books.end()) {
        Node* book = *it;

        if (shouldDeleteByRating(book, r)) {
            int deletedNodes = countSubtree(book);
            deleteSubtree(book);
            it = books.erase(it);
            treeSize -= deletedNodes;
        } else {
            ++it;
        }
    }
}

void Tree::precursores(){
    precursoresRec(rootNode);
}

void Tree::precursoresRec(Node* node){
    if (node == nullptr) {
        return;
    }

    if (isMainBook(node) && isPrecursor(node)) {
        cout << childText(node, "id") << endl;
    }

    for (Node* child : node->children) {
        precursoresRec(child);
    }
}

bool Tree::isPrecursor(Node* book){
    int bookYear = toInt(childText(book, "publication_year"));

    if (bookYear < 0) {
        return false;
    }

    Node* similarBooks = firstChild(book, "similar_books");

    if (similarBooks == nullptr || similarBooks->children.empty()) {
        return false;
    }

    for (Node* similarBook : similarBooks->children) {
        int similarYear = toInt(childText(similarBook, "publication_year"));

        if (similarYear < 0) {
            return false;
        }

        if (similarYear <= bookYear) {
            return false;
        }
    }

    return true;
}