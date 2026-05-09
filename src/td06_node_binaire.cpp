#include "td06_node_binaire.hpp"
#include <iostream>

void pretty_print_left_right(Node const &node, std::string const &prefix, bool is_left)
{
    if (node.right)
    {
        pretty_print_left_right(*node.right, prefix + (is_left ? "| " : " "), false);
    }
    std::cout << prefix << (is_left ? "+-- " : "+-- ") << node.value << std::endl;
    if (node.left)
    {
        pretty_print_left_right(*node.left, prefix + (is_left ? " " : "| "), true);
    }
}

void pretty_print_left_right(Node const &node)
{
    pretty_print_left_right(node, "", true);
}

Node *create_node(int value)
{
    Node *n = new Node; // réserve la mémoire sur la heap
    n->value = value;   // remplit la valeur
    n->left = nullptr;  // fils gauche = rien
    n->right = nullptr; // fils droit = rien
    return n;           // retourne l'adresse du nœud créé
}

bool Node::is_leaf() const {
    return left == nullptr && right == nullptr;
}

void Node::insert(int value) {
    if (value < this->value) {       // ça va à gauche
        if (left == nullptr)         // la place est libre
            left = create_node(value);
        else                         // la place est prise → on demande au fils de gérer
            left->insert(value);
    } else {                         // ça va à droite
        if (right == nullptr)        // la place est libre
            right = create_node(value);
        else                         // la place est prise → on demande au fils de gérer
            right->insert(value);
    }
}

int Node::height() const {
    if (is_leaf())           // cas de base : je suis tout seul
        return 1;

    int left_height  = left  ? left->height()  : 0;  // hauteur du côté gauche
    int right_height = right ? right->height() : 0;  // hauteur du côté droit

    return 1 + std::max(left_height, right_height);  // je prends le plus grand + moi
}