#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <map>
#include <unordered_set>
#include <fstream>
#include "td06_node_binaire.hpp"

void main()
{
    Node *root = create_node(10);
    root->insert(5);
    root->insert(15);
    root->insert(3);
    root->insert(7);
    root->insert(20);

    // Affichage visuel de l'arbre
    pretty_print_left_right(*root);

    // is_leaf
    std::cout << "\n=== is_leaf ===\n";
    std::cout << "10 est une feuille ? " << root->is_leaf() << "\n";             // 0
    std::cout << "3  est une feuille ? " << root->left->left->is_leaf() << "\n"; // 1

    // height
    std::cout << "\n=== height ===\n";
    std::cout << "Hauteur de l'arbre : " << root->height() << "\n"; // 3
}