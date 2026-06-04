#include <iostream>
#include <string>
#include <filesystem>
#include "tree.hpp"
#include "pugixml.hpp"

using namespace std;
namespace fs = std::filesystem;

string textOf(pugi::xml_node node, const char* tag) {
    pugi::xml_node child = node.child(tag);

    if (!child) {
        return "";
    }

    return child.text().as_string();
}

void cargarLibro(Tree& tree, const string& rutaArchivo) {
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(rutaArchivo.c_str());

    if (!result) {
        cerr << "Error al leer XML: " << rutaArchivo << endl;
        return;
    }

    pugi::xml_node rootXml = doc.child("GoodreadsResponse");

    if (!rootXml) {
        cerr << "No se encontro <GoodreadsResponse> en: " << rutaArchivo << endl;
        return;
    }

    pugi::xml_node bookXml = rootXml.child("book");

    if (!bookXml) {
        cerr << "No se encontro <book> en: " << rutaArchivo << endl;
        return;
    }

    Tree::Node* book = tree.insert(tree.root(), "book");

    tree.insert(book, "id", textOf(bookXml, "id"));
    tree.insert(book, "title", textOf(bookXml, "title"));
    tree.insert(book, "isbn", textOf(bookXml, "isbn"));
    tree.insert(book, "publication_year", textOf(bookXml, "publication_year"));
    tree.insert(book, "language", textOf(bookXml, "language_code"));
    tree.insert(book, "description", textOf(bookXml, "description"));
    tree.insert(book, "average_rating", textOf(bookXml, "average_rating"));
    tree.insert(book, "num_pages", textOf(bookXml, "num_pages"));

    Tree::Node* similarBooks = tree.insert(book, "similar_books");

    pugi::xml_node similarBooksXml = bookXml.child("similar_books");

    if (similarBooksXml) {
        for (pugi::xml_node similarXml : similarBooksXml.children("book")) {
            Tree::Node* similarBook = tree.insert(similarBooks, "similar_book");

            tree.insert(similarBook, "title", textOf(similarXml, "title"));
            tree.insert(similarBook, "isbn", textOf(similarXml, "isbn"));
            tree.insert(similarBook, "publication_year", textOf(similarXml, "publication_year"));
        }
    }
}

void cargarCarpeta(Tree& tree, const string& carpeta) {
    for (const auto& entry : fs::directory_iterator(carpeta)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            cargarLibro(tree, entry.path().string());
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Uso:" << endl;
        cout << "./programa carpeta_xml listar" << endl;
        cout << "./programa carpeta_xml borrar_ratings r" << endl;
        cout << "./programa carpeta_xml precursores" << endl;
        return 1;
    }

    string carpeta = argv[1];
    string comando = argv[2];

    Tree tree;

    cargarCarpeta(tree, carpeta);

    if (comando == "listar") {
        tree.listar();
    }
    else if (comando == "borrar_ratings") {
        if (argc < 4) {
            cout << "Falta el parametro r." << endl;
            return 1;
        }

        double r = stod(argv[3]);

        tree.borrar_ratings(r);
        tree.listar();
    }
    else if (comando == "precursores") {
        tree.precursores();
    }
    else {
        cout << "Comando no reconocido." << endl;
        return 1;
    }

    return 0;
}