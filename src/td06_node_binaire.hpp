#pragma once
#include <iostream>

struct Node {
int value;
Node* left { nullptr };
Node* right { nullptr };

bool is_leaf() const;
void Node::insert(int value);
int Node::height() const;
};

void pretty_print_left_right(Node const& node, std::string const& prefix, bool is_left);
void pretty_print_left_right(Node const& node);
Node* create_node(int value);