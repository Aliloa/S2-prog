#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <cctype>
#include <stack>

std::vector<std::string> temporary_tokens{}; // Elements du calcul

enum class Operator
{
    ADD,
    SUB,
    MUL,
    DIV
};
enum class TokenType
{
    OPERATOR,
    OPERAND
};

struct Token
{
    TokenType type;
    float value;
    Operator op;
};

// Création d'un token "nombre"
Token make_token(float value)
{
    Token t;
    t.type = TokenType::OPERAND;
    t.value = value;
    return t;
}

// Création d'un token "opérateur"
Token make_token(Operator op)
{
    Token t;
    t.type = TokenType::OPERATOR;
    t.op = op;
    return t;
}

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

std::vector<Token> tokenize(std::vector<std::string> const &words)
{
    std::vector<Token> tokens;
    for (const std::string &word : words)
    {
        if (is_floating(word))
        {
            tokens.push_back(make_token(std::stof(word)));
        }
        else
        {
            Operator op;
            if (word == "+")
                op = Operator::ADD;
            else if (word == "-")
                op = Operator::SUB;
            else if (word == "*")
                op = Operator::MUL;
            else if (word == "/")
                op = Operator::DIV;
            else
            {
                std::cerr << "Erreur : opérateur inconnu '" << word << "'\n";
                continue; // ou throw exception
            }

            tokens.push_back(make_token(op));
        }
    }
    return tokens;
}

void npi_evaluate(std::vector<Token> const &tokens)
{
    std::stack<float> stack;
    for (const Token &token : tokens)
    {
        if (token.type == TokenType::OPERAND)
        {
            stack.push(token.value);
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

            // On peut faire un switch et des case vu que mtn c un struct ca pourrait simplifier le code mais flm
            if (token.op == Operator::ADD)
                result = leftOperand + rightOperand;
            else if (token.op == Operator::SUB)
                result = leftOperand - rightOperand;
            else if (token.op == Operator::MUL)
                result = leftOperand * rightOperand;
            else if (token.op == Operator::DIV)
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

    temporary_tokens = split_string(calcul);

    // ca marche pas ca jsp pk (j'avais mis les tokens en variable globale de base...)
    //  tokenize(temporary_tokens);
    //  npi_evaluate(tokens);

    npi_evaluate(tokenize(temporary_tokens)); // faut mettre ca comme ca

    return 0;
}
