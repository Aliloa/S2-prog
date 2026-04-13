#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "ScopedTimer.hpp"

int main()
{
    std::vector<int> tab{};
    for (int i = 0; i < 10; i++)
    {
        int b = rand() % 100 + 1;
        tab.push_back(b);
        std::cout << tab[i] << std::endl;
    }

     std::cout << "taille du tableau: " << tab.size() << std::endl;
    return 0;
}