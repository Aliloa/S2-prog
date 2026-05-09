# Exercice 1 - Commandes utilisées:
./osmGraph.exe extract data/test.osm data/test_extract.graph           
./osmGraph.exe simplify data/test_extract.graph
./osmGraph.exe visualize data/test_extract.graph

# Exercice 2 - Lecture de code et compréhension

## Identifiez où sont définies les structures principales du graphe (WeightedGraph / PositionedGraph) et expliquez brièvement leur rôle et comment elles sont utilisées.

- WeightedGraph se trouve dans src/dataStrucutre/weightedGraph.hpp
Il définit la structure de données centrale du projet : un graphe pondéré qui modélise un réseau routier.
C'est le fichier qui définit comment stocker une carte : des points reliés entre eux par des chemins qui ont chacun un poids (la distance ou le temps par exemple)

Il contient de structures: WeightedGraph qui stocke les connexions entre points et WeightedArc qui stocke la destination et le poids

- PositionedGraph se trouve dans src/osm/positionnedGraph.hpp
Ce fichier manipule la position des coordonnées x/y de chaque point, car le graphe de base ne stockait que les connexions sans savoir où les points se trouvent sur la carte

Il contient un seule structure: PositionedGraph qui contient un WeightedGraph + les coordonnées (x, y) de chaque point

## Expliquez en quelques lignes le rôle des modules:

- Extraction OSM:
Lit un fichier .osm et en extrait le graphe pour le sauvegarder dans un fichier .graph. Il transforme la donnée géographique brute en structure utilisable

- Simplification:
Prend le graphe extrait et le simplifie en supprimant les noeuds inutiles (par exemple les points intermédiaires sur une route droite qui n'ont qu'un seul voisin)

- Visualisation :
Affiche le graphe dirrectement sur l'écran pour pouvoir le visualiser

## Expliquez ce que vous comprenez des différentes étapes de simplification implémentées (fichier src/simplification/simplify.cpp) et les raisons pour lesquelles elles sont utilisées (leur impact sur la structure du graphe, les avantages/inconvénients, etc.).

Le but c'est de réduire le nombre de noeuds du graphe pour le rendre plus léger et lisible

1. keep_only_largest_connected_component(graph);
Garde que la plus grande composante et supprime tous les petits bouts de routes isolés qui ne sont pas reliés au réseau principal

2. remove_small_ending_edge(graph, 10.0);
Supprime les petites impasses en enlevant les noeuds en bout de route qui sont trop proches de leur unique voisin

3. remove_degree_two_nodes_by_angle_threshold(graph, 30);
Supprime les noeuds inutiles sur les routes droites

4. group_nodes_by_connection_depth_and_proximity(graph, 10.0, 6);
Fusionner les carrefours complexes, les grands carrefours ou ronds-points sont souvent représentés par plein de noeuds très proches, alors on les fusionne en un seul point

5. remove_degree_two_nodes_by_angle_threshold(graph, 30);
Deuxième passe de suppression des noeuds inutiles, la fusion de l'étape 4 peut créer de nouveaux noeuds à 2 voisins, donc on refait l'étape 3 une deuxième fois