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
// Funcion que utilizara listarRec, ShouldDeleteByRating y precursoresRec para determinar si un nodo es un libro principal, es decir, si su tag es "book" y su padre es la raíz.
bool Tree::isMainBook(Node* node){
    return node != nullptr &&
           node->data.tag == "book" &&
           node->parent == rootNode;
}
//Funcion listar que lista los ID de los libros principales. Recorre el arbol en preorder e imprime los ID de los libros principales.
void Tree::listar(){
    listarRec(rootNode); // Llama a la función recursiva para listar los libros principales, comenzando desde la raiz del arbol.
}
// Funcion listarRec, que recorre el arbol en preorder e imprime los ID de los libros principales.
void Tree::listarRec(Node* node){
    if (node == nullptr) { // Si el nodo es nulo, se retorna sin hacer nada.
        return;
    }

    if (isMainBook(node)) { // Si el nodo es un libro principal, se imprime su ID. El ID se obtiene buscando el texto del hijo con tag "id" del nodo.
        cout << childText(node, "id") << endl;
    }

    for (Node* child : node->children) { // Se recorre el vector de hijos del nodo y se llama recursivamente a listarRec para cada hijo, lo que permite recorrer todo el arbol en preorder.
        listarRec(child);
    }
}

//Funcion borrar_ratings, que elimina de la raiz todos los libros principales con rating menor o igual a r. Recorre el vector de hijos de la raiz y elimina los nodos que cumplen la condición, actualizando el tamaño del árbol.
void Tree::borrar_ratings(double r) {
    vector<Node*>& books = rootNode->children; // Se obtiene una referencia al vector de hijos de la raiz, que son los libros principales.

    auto it = books.begin(); // Se crea un iterador para recorrer el vector de libros principales.

    while (it != books.end()) { // Se recorre el vector de libros principales utilizando el iterador.
        Node* book = *it;

        if (shouldDeleteByRating(book, r)) { // Si el libro cumple la condición de borrarse por rating, se cuenta la cantidad de nodos en el subarbol que tiene a book como raiz utilizando la función countSubtree, se elimina el subarbol utilizando la función deleteSubtree, se borra el nodo del vector de hijos de la raiz utilizando el iterador, y se actualiza el tamaño del árbol restando la cantidad de nodos eliminados.
            int deletedNodes = countSubtree(book);
            deleteSubtree(book);
            it = books.erase(it);
            treeSize -= deletedNodes;
        } else { // Si el libro no cumple la condición de borrarse por rating, se avanza el iterador al siguiente libro principal.
            ++it;
        }
    }
}

// Funcion que utilizara borrar_ratings para determinar si un libro debe borrarse por rating, es decir, si es un libro principal y su rating es menor o igual a r.
bool Tree::shouldDeleteByRating(Node* book, double r){
    if (!isMainBook(book)) { // Si el nodo no es un libro principal, no se debe borrar por rating, por lo que se retorna false.
        return false;
    }

    double rating = toDouble(childText(book, "average_rating")); // Se obtiene el rating del libro buscando el texto del hijo con tag "average_rating" del nodo y convirtiendolo a double.

    if (rating < 0) { // Si el rating es menor a 0, se asume que no se pudo obtener un rating válido, por lo que no se debe borrar por rating, y se retorna false.
        return false;
    }

    return rating <= r; // Si el rating es menor o igual a r, se debe borrar por rating, por lo que se retorna true. De lo contrario, se retorna false.
}

// Funcion precursores, que lista los ID de libros cuyos similares fueron publicados después. Recorre el arbol en preorder e imprime los ID de los libros principales que tienen al menos un libro similar publicado después.
void Tree::precursores(){
    precursoresRec(rootNode);
}
// Funcion precursoresRec, que recorre el arbol en preorder e imprime los ID de los libros principales que tienen al menos un libro similar publicado después.
void Tree::precursoresRec(Node* node){
    if (node == nullptr) { // Si el nodo es nulo, se retorna sin hacer nada.
        return;
    }

    if (isMainBook(node) && isPrecursor(node)) { // Si el nodo es un libro principal y es precursor, se imprime su ID. El ID se obtiene buscando el texto del hijo con tag "id" del nodo.
        cout << childText(node, "id") << endl;
    }

    for (Node* child : node->children) { // Se recorre el vector de hijos del nodo y se llama recursivamente a precursoresRec para cada hijo, lo que permite recorrer todo el arbol en preorder.
        precursoresRec(child);
    }
}
//Funcion que utilizara precursoresRec para determinar si un libro es precursor, es decir, si es un libro principal y tiene al menos un libro similar publicado después.
bool Tree::isPrecursor(Node* book){
    int bookYear = toInt(childText(book, "publication_year")); // Se obtiene el año de publicación del libro buscando el texto del hijo con tag "publication_year" del nodo y convirtiendolo a int.

    if (bookYear < 0) { // Si el año de publicación es menor a 0, se asume que no se pudo obtener un año de publicación válido, por lo que no se puede determinar si el libro es precursor, y se retorna false.
        return false;
    }

    Node* similarBooks = firstChild(book, "similar_books"); // Se busca el nodo de similares del libro utilizando la función firstChild para buscar el primer hijo directo con tag "similar_books".

    if (similarBooks == nullptr || similarBooks->children.empty()) { // Si no se encuentra el nodo de similares o si no tiene hijos, se asume que el libro no tiene similares publicados después, por lo que no es precursor, y se retorna false.
        return false;
    }

    for (Node* similarBook : similarBooks->children) { // Se recorre el vector de hijos del nodo de similares, que son los libros similares al libro principal.
        int similarYear = toInt(childText(similarBook, "publication_year"));

        if (similarYear < 0) { // Si el año de publicación del libro similar es menor a 0, se asume que no se pudo obtener un año de publicación válido para ese libro similar, por lo que no se puede determinar si el libro principal es precursor basándose en ese libro similar, y se continúa con el siguiente libro similar.
            return false;
        }

        if (similarYear <= bookYear) { // Si el año de publicación del libro similar es menor o igual al año de publicación del libro principal, se asume que el libro similar no fue publicado después del libro principal, por lo que el libro principal no es precursor, y se retorna false.
            return false;
        }
    }

    return true; // Si se recorrieron todos los libros similares y ninguno fue publicado antes o el mismo año que el libro principal, se asume que el libro principal es precursor, por lo que se retorna true.
}