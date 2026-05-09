# Nous voulons un cmake "récent" pour utiliser les dernières fonctionnalités 
cmake_minimum_required(VERSION 4.0)
# La version du C++ que l'on souhaite utiliser (dans notre cas C++17)
set(CMAKE_CXX_STANDARD 17)

# Le nom du projet
project(IMAC_project)

#Ajouter les fichiers map dans le build pour pouvoir le lancer
configure_file(map.txt map.txt COPYONLY)
configure_file(input_guard_patrol.txt input_guard_patrol.txt COPYONLY)

# On indique que l'on veut créer un exécutable "helloImac" compilé à partir du fichier helloImac.cpp
add_executable(td src/td06.cpp)
