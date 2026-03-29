#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "ScopedTimer.hpp"

//----------------------------------------------TRI A BULLE

// vérifie si un tableau est trié par ordre croissant ou décroissant
bool is_sorted(std::vector<int> const &vec) { return std::is_sorted(vec.begin(), vec.end()); }

void bubble_sort(std::vector<int> &vec)
{
    while (!is_sorted(vec))
    {
        for (size_t i = 0; i < vec.size() - 1; i++)
        {
            if (vec[i] > vec[i + 1])
            {
                std::swap(vec[i], vec[i + 1]);
            }
        }
    }
}

//---------------------------------------TRI FUSION
// phase de fusion
void merge_sort_merge(std::vector<int> &vec, size_t const left, size_t const middle, size_t const right)
{
    std::vector<int> temporary_tab;

    size_t i = left;
    size_t j = middle + 1;

    while (i <= middle && j <= right)
    {
        if (vec[i] <= vec[j])
        {
            temporary_tab.push_back(vec[i]);
            i++;
        }
        else
        {
            temporary_tab.push_back(vec[j]);
            j++;
        }
    }

    // partie gauche
    while (i <= middle)
    {
        temporary_tab.push_back(vec[i]);
        i++;
    }

    // partie droite
    while (j <= right)
    {
        temporary_tab.push_back(vec[j]);
        j++;
    }

    // recopie dans vec
    for (size_t k = 0; k < temporary_tab.size(); k++)
    {
        vec[left + k] = temporary_tab[k];
    }
}

// phase de division
void merge_sort(std::vector<int> &vec, size_t const left, size_t const right)
{
    if (left >= right)
        return;

    size_t middle = left + (right - left) / 2;

    merge_sort(vec, left, middle);
    merge_sort(vec, middle + 1, right);

    merge_sort_merge(vec, left, middle, right);
}

void merge_sort(std::vector<int> &vec)
{
    merge_sort(vec, 0, vec.size() - 1);
}

//----------------------------------------Comparaison des algorithmes de tri
std::vector<int> generate_random_vector(size_t const size, int const max = 100)
{
    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), [&max]()
                  { return std::rand() % max; });
    return vec;
}

//----------------------------------------- Dichotomie

int search(const std::vector<int>& vec, int value) {
    int left = 0;
    int right = vec.size() - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (vec[middle] == value) {
            return middle; // trouvé
        }
        else if (vec[middle] < value) {
            left = middle + 1; // droite
        }
        else {
            right = middle - 1; // gauche
        }
    }

    return -1; // pas trouvé
}

int main(int argc, char const *argv[])
{
    std::vector<int> array{12, 3, 4, 51, 67, 7, 8, 9, 101, 8, 8};

     std::cout << "taille du tableau: " << array.size() << std::endl;
    // -----------------------------TRI A BULLE
    std::vector<int> bubble_array{array};

    {
        ScopedTimer timer("Temps de tri a bulles");
        bubble_sort(bubble_array);
    }

    // for (int i = 0; i < bubble_array.size(); i++)
    // {
    //     std::cout << bubble_array[i] << std::endl;
    // };

    // ----------------------------TRI FUSION
    std::vector<int> fusion_array{array};

    {
        ScopedTimer timer("Temps de tri fusion");
        merge_sort(fusion_array);
    }

    // for (int i = 0; i < fusion_array.size(); i++)
    // {
    //     std::cout << fusion_array[i] << std::endl;
    // };

    // ----------------------------BIBLIOTHEQUE STANDART
    std::vector<int> std_array{array};

    {
        ScopedTimer timer("Temps de std::sort");
        std::sort(std_array.begin(), std_array.end());
    }

    // for (int i = 0; i < std_array.size(); i++)
    // {
    //     std::cout << std_array[i] << std::endl;
    // }

    //-------------------------------------------dichotomie
    // tableau 1
    std::vector<int> tab1 {1, 2, 2, 3, 4, 8, 12};
    std::cout << "Indice de 8 : " << search(tab1, 8) << std::endl;

    // tableau 2
    std::vector<int> tab2 {1, 2, 3, 3, 6, 14, 12, 15};
    std::cout << "Indice de 15 : " << search(tab2, 15) << std::endl;

    // tableau 3
    std::vector<int> tab3 {2, 2, 3, 4, 5, 8, 12, 15, 16};
    std::cout << "Indice de 16 : " << search(tab3, 16) << std::endl;

    // tableau 4
    std::vector<int> tab4 {5, 6, 7, 8, 9, 10, 11, 12, 13};
    std::cout << "Indice de 6 : " << search(tab4, 6) << std::endl;

    // tableau 5
    std::vector<int> tab5 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "Indice de 10 : " << search(tab5, 10) << std::endl;

    return 0;
}
