#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <cctype>
#include <stack>

std::vector<std::string> tokens{}; // Elements du calcul

// séparer les éléments (mots) de la chaîne de caractères en utilisant les espaces
std::vector<std::string> split_string(std::string const &s)
{
    std::istringstream in(s); // transforme une chaîne en flux de caractères, cela simule un flux comme l'est std::cin

    // l’itérateur va lire chaque element de "in", comme un flux d'entrée, chaque élément est séparé par un espace
    return std::vector<std::string>(std::istream_iterator<std::string>(in), std::istream_iterator<std::string>());
}

// dire si un token représente un nombre ou non
bool is_floating(std::string const &s)
{
    for (char c : s)
    {
        if (std::isdigit(c))
        {
            continue;
        }
        else if (c == '.')
        {
            continue;
        }
        return false;
    }
    return true;
}

//
float npi_evaluate(std::vector<std::string> const &tokens)
{
    std::stack<float> stack;
    for (const std::string &token : tokens)
    {
        if (is_floating(token))
        {
            stack.push(std::stof(token)); // stof pour convertir le string en float
        }
        else
        {
            // Je récupère l'élément en haut de la pile
            float rightOperand{stack.top()};
            // Je l'enlève de la stack (la méthode top ne fait que lire l’élément en dessus de la pile)
            stack.pop();
            float leftOperand{stack.top()};
            stack.pop();
            // Il faut ensuite en fonction de l'opérateur calculer le résultat pour le remettre dans la pile
            float result;

            if (token == "+")
                result = leftOperand + rightOperand;
            else if (token == "-")
                result = leftOperand - rightOperand;
            else if (token == "*")
                result = leftOperand * rightOperand;
            else if (token == "/")
                result = leftOperand / rightOperand;
            stack.push(result);
        }
    }

    if (stack.size() == 1)
    {
        std::cout << "Resultat " << stack.top() << std::endl;
    }
    else
    {
        std::cout << "Euh il reste plus d'un element dans le tableau l'expression est invalide " << std::endl;
    }
}

int main()
{
    std::cout << "Entrez un calcul en NPI: " << std::endl;
    std::string calcul;
    std::getline(std::cin, calcul);
    std::cout << "Vous avez mis " << calcul << std::endl;

    tokens = split_string(calcul);

    npi_evaluate(tokens);

    return 0;
}
