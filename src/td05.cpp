#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <map>
#include <unordered_set>
#include <fstream>

//------------------------------------------Hashing simple
size_t folding_string_hash(std::string const &s, size_t max)
{
    size_t hash{0};
    for (size_t i{0}; i < s.size(); i++)
    {
        hash += s[i];
        hash %= max;
    }
    return hash;
}

//----------------------------------------------Hashing Ordonné

size_t folding_string_ordered_hash(std::string const &s, size_t max)
{
    size_t hash{0};
    for (size_t i{0}; i < s.size(); i++)
    {
        hash = ((hash + s[i]) * (i + 1)) % max;
    }
    return hash;
}

//----------------------------------------------Guard Patrol
// utiliser un map (?) (on peut faire vect, mais map c plus simple askip)

enum Direction
{
    Haut,
    Droite,
    Bas,
    Gauche
};

struct Position
{
    int x;
    int y;
};

bool operator==(Position const &a, Position const &b)
{
    return a.x == b.x && a.y == b.y;
}

std::ostream &operator<<(std::ostream &os, Position const &a)
{
    return os << "(" << a.x << ", " << a.y << ")";
}

Position operator+(Position const &a, Position const &b)
{
    return {a.x + b.x, a.y + b.y};
}

Position &operator+=(Position &a, Position const &b)
{
    a = a + b;
    return a;
}

Position operator+(Position &p, Direction const &d)
{
    switch (d)
    {
    case Haut:
        p.y -= 1;
        break;
    case Bas:
        p.y += 1;
        break;
    case Droite:
        p.x += 1;
        break;
    case Gauche:
        p.x -= 1;
        break;
    }
    return p;
}

Position &operator+=(Position &a, Direction d)
{
    a = a + d;
    return a;
}

Direction turn_right(Direction d)
{
    switch (d)
    {
    case Haut:
        d = Droite;
        break;
    case Bas:
        d = Gauche;
        break;
    case Droite:
        d = Bas;
        break;
    case Gauche:
        d = Haut;
        break;
    }
    return d;
}

struct Input_Structure
{
    Position init_pos;
    Direction init_dir;
    std::map<std::pair<int, int>, int> obstacles;
    int width;
    int height;
};

Input_Structure read_input(std::istream &input_stream) // prend un fichier avec les . et # et ajoute tous les élements dans mes tableaux associés
{
    Input_Structure result;
    int y = 0; // numéro de ligne courante
    result.width = 0;
    result.height = 0;
    for (std::string line{}; std::getline(input_stream, line, '\n') && line != "";)
    {
        result.width = line.size(); // largeur = nombre de caractères par ligne
        for (int x = 0; x < line.size(); x++)
        {
            char c = line[x];
            if (c == '#')
                result.obstacles[{x, y}] = 1;
            else if (c == '^')
            {
                result.init_pos = {x, y};
                result.init_dir = Direction::Haut;
            }
            else if (c == 'v')
            {
                result.init_pos = {x, y};
                result.init_dir = Direction::Bas;
            }
            else if (c == '<')
            {
                result.init_pos = {x, y};
                result.init_dir = Direction::Gauche;
            }
            else if (c == '>')
            {
                result.init_pos = {x, y};
                result.init_dir = Direction::Droite;
            }
        }
        y++;
    }
    result.height = y; // nombre de lignes lues
    return result;
}

// définir une fonction de hachage pour la structure Position
namespace std
{
    template <>
    struct hash<Position>
    {
        std::size_t operator()(const Position &pos) const
        {
            std::size_t h1 = std::hash<int>{}(pos.x);
            std::size_t h2 = std::hash<int>{}(pos.y);
            return h1 ^ (h2 << 1);
        }
    };
}

struct WalkResult
{
    Position final_position;
    int steps;
    std::unordered_set<Position> visited_positions;
};

WalkResult walk(Input_Structure input)
{
    WalkResult result;

    // init
    Position pos = input.init_pos;
    Direction dir = input.init_dir;

    result.visited_positions.insert(pos); // insérer positiond de départ

    while (true)
    {
        // calculer la case devant le garde
        int dx = 0;
        int dy = 0;
        if (dir == Direction::Haut)
        {
            dx = 0;
            dy = -1;
        }
        if (dir == Direction::Bas)
        {
            dx = 0;
            dy = 1;
        }
        if (dir == Direction::Gauche)
        {
            dx = -1;
            dy = 0;
        }
        if (dir == Direction::Droite)
        {
            dx = 1;
            dy = 0;
        }

        // La case devant le garde
        Position next;
        next.x = pos.x + dx;
        next.y = pos.y + dy;

        // limite de la carte?
        if (next.x < 0 || next.y < 0 || next.x >= input.width || next.y >= input.height)
        {
            // Le garde sort de la carte, on arrête
            result.final_position = pos;
            break;
        }

        // Obstacles
        if (input.obstacles.count({next.x, next.y}) > 0)
        {
            // Obstacle -> tourner à droite
            if (dir == Direction::Haut)
                dir = Direction::Droite;
            else if (dir == Direction::Droite)
                dir = Direction::Bas;
            else if (dir == Direction::Bas)
                dir = Direction::Gauche;
            else if (dir == Direction::Gauche)
                dir = Direction::Haut;
        }
        else
        {
            // Pas d'obstacle -> on avance
            pos = next;
            result.steps += 1;
            result.visited_positions.insert(pos); // on marque la case comme visitée
        }
    }
    return result;
}

//------------- Q4 Trouver les boucles
struct GuardState
{
    int x, y;
    Direction dir;
};

// Hash
bool operator==(const GuardState &a, const GuardState &b)
{
    return a.x == b.x && a.y == b.y && a.dir == b.dir;
}

namespace std
{
    template <>
    struct hash<GuardState>
    {
        size_t operator()(const GuardState &s) const
        {
            return s.x * 10000 + s.y * 10 + static_cast<int>(s.dir);
        }
    };
}

bool is_loop(Input_Structure input)
{
    std::unordered_set<GuardState> vus; // les états déjà vus

    Position pos = input.init_pos;
    Direction dir = input.init_dir;

    while (true)
    {
        // vérifier si on a déjà vu cet état
        GuardState etat = {pos.x, pos.y, dir};
        if (vus.count(etat) > 0)
            return true; // boucle !
        vus.insert(etat);

        int dx = 0, dy = 0;
        if (dir == Direction::Haut)
        {
            dx = 0;
            dy = -1;
        }
        if (dir == Direction::Bas)
        {
            dx = 0;
            dy = 1;
        }
        if (dir == Direction::Gauche)
        {
            dx = -1;
            dy = 0;
        }
        if (dir == Direction::Droite)
        {
            dx = 1;
            dy = 0;
        }

        Position next;
        next.x = pos.x + dx;
        next.y = pos.y + dy;

        if (next.x < 0 || next.y < 0 || next.x >= input.width || next.y >= input.height)
            return false; // il sort → pas de boucle

        if (input.obstacles.count({next.x, next.y}) > 0)
        {
            if (dir == Direction::Haut)
                dir = Direction::Droite;
            else if (dir == Direction::Droite)
                dir = Direction::Bas;
            else if (dir == Direction::Bas)
                dir = Direction::Gauche;
            else if (dir == Direction::Gauche)
                dir = Direction::Haut;
        }
        else
        {
            pos = next;
        }
    }
}

int compter_boucles(Input_Structure input)
{
    int count = 0;

    // recuperer les cases visitées par le garde normalement
    WalkResult original = walk(input);

    for (Position p : original.visited_positions)
    {
        // pas d'obstacle sur la position de départ
        if (p.x == input.init_pos.x && p.y == input.init_pos.y)
            continue;

        // ajouter un obstacle temporaire
        input.obstacles.insert({{p.x, p.y}, 1});

        if (is_loop(input))
            count++;

        // enlèver l'obstacle
        input.obstacles.erase({p.x, p.y});
    }

    return count;
}

int main()
{
    //---Guard Patrol
    {
        std::ifstream file("map.txt"); // Ouvrir le fichier de la carte

        if (!file.is_open())
        {
            std::cout << "ERREUR : fichier map.txt introuvable !" << std::endl;
            return 0;
        }

        Input_Structure input = read_input(file);
        WalkResult result = walk(input);
        std::cout << "Positions visites : " << result.visited_positions.size() << std::endl;
        int nb_boucles = compter_boucles(input);
        std::cout << "Boucles possibles : " << nb_boucles << std::endl;
    }

    std::string s;
    std::cin >> s;
    //---Hashing simple
    {
        size_t hash = folding_string_hash(s, 1024);
        std::cout << "hash : " << hash << std::endl;
    }
    //---Hashing Ordonné
    {
        size_t ordered_hash = folding_string_ordered_hash(s, 1024);
        std::cout << "oredred hash : " << ordered_hash << std::endl;
    }

    return 0;
}