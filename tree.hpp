#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

//Struct de los datos que iran en cada nodo, donde tag sera el string del tag, por ejemplo, "title", y text sera el texto del nodo, por ejemplo, "Harry potter".
struct XmlData {
    string tag;
    string text;
};

//Clase del arbol
class Tree {
//Atributos publicos del arbol
    public:
    // struct del nodo del arbol, que contiene un XmlData, un puntero al padre y un vector de punteros a los hijos.
    struct Node {
        XmlData data;
        Node* parent;
        vector<Node*> children;

        Node(string tag, string text = "", Node* p = nullptr);
    };


//Atributos y metodos privados del arbol
    private:
    Node* rootNode; // Puntero a la raíz del árbol
    int treeSize; // Cantidad de nodos en el árbol

    void deleteSubtree(Node* node); // Elimina un nodo y todos sus descendientes.
    int countSubtree(Node* node); // Cuenta la cantidad de nodos en el subarbol que tiene a nodo como raiz.

    void listarRec(Node* node); // Funcion recursiva para listar los libros principales. Recorre el arbol en preorder e imprime los ID de los libros principales.
    void precursoresRec(Node* node); // Funcion recursiva para listar los precursores. Recorre el arbol en preorder e imprime los ID de los libros precursores.

    bool isMainBook(Node* node); // Determina si un nodo es un libro principal, es decir, si su tag es "book" y su padre es la raíz.
    bool shouldDeleteByRating(Node* book, double r); // Determina si un libro debe borrarse por rating, es decir, si es un libro principal y su rating es menor o igual a r.
    bool isPrecursor(Node* book); // Determina si un libro es precursor, es decir, si es un libro principal y tiene al menos un libro similar publicado despues.

    int toInt(string value); // Convierte un string a int.
    double toDouble(string value); // Convierte un string a double.

public:
    Tree(); // Constructor del arbol, que inicializa la raiz y el tamaño.
    ~Tree(); // Destructor del arbol, que elimina la raiz y todos sus descendientes.

    bool isEmpty(); // Determina si el arbol esta vacío y devuelve booleano.
    int size(); // Retorna la cantidad de nodos en el arbol.

    Node* root(); // Retorna un puntero a la raiz del arbol.

    Node* insert(Node* parent, string tag, string text = ""); // Inserta un nuevo nodo con el tag y texto especificados como hijo de parent. Devuelve un puntero al nuevo nodo, o nullptr si parent es nullptr.

    Node* firstChild(Node* node, string tag); // Busca el primer hijo directo de node con el tag especificado. Devuelve un puntero al nodo encontrado, o nullptr si no se encuentra.
    string childText(Node* node, string tag); // Retorna el texto del primer hijo directo de node con el tag especificado. Devuelve una cadena vacía si no se encuentra el hijo o si el nodo es nullptr.

    void listar(); // Lista los ID de los libros principales. Recorre el arbol en preorder e imprime los ID de los libros principales.
    void borrar_ratings(double r); // Elimina de la raiz todos los libros principales con rating menor o igual a r. Recorre el vector de hijos de la raiz y elimina los nodos que cumplen la condición, actualizando el tamaño del árbol.
    void precursores(); // Lista los ID de libros cuyos similares fueron publicados después. Recorre el arbol en preorder e imprime los ID de los libros principales que tienen al menos un libro similar publicado después.
};

#endif