//Exercice 3 - Dijkstra dans le projet OsmGraph
//code de graphUtils.cpp

#include "graphUtils.hpp"
#include "weightedGraph.hpp"

#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>

using DataStructure::WeightedGraph;
using DataStructure::IDType;

void DFS(WeightedGraph const& graph, IDType const start, std::function<void(IDType const)> const& callback, std::function<bool(IDType const)> const& should_explore_neighbors) {
    std::set<IDType> visited {};
    std::stack<IDType> to_visit {};
    to_visit.push(start);
    while (!to_visit.empty()) {
        auto const current { to_visit.top() };
        to_visit.pop();

        if (visited.contains(current)) {
            continue;
        }

        visited.insert(current);
        callback(current);

        if (should_explore_neighbors(current)) {
            for (auto const& edge : graph.get_neighbors(current)) {
                to_visit.push(edge.to);
            }
        }
    }
}

void print_DFS(WeightedGraph const& graph, IDType const start) {
    /* TODO */
}


template<class T>
using min_priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

// map of node id to pair of cost and parent node id (for path reconstruction)
std::unordered_map<IDType, std::pair<float, IDType>> Dijkstra(WeightedGraph const& graph, IDType const start, IDType const end) {
    std::unordered_map<IDType, std::pair<float, IDType>> distances {};
    min_priority_queue<std::pair<float, IDType>> to_visit {};

    to_visit.push({0.0f, start});
    distances[start] = {0.0f, start};

    while (!to_visit.empty()) {
        auto const [current_cost, current_node] = to_visit.top();
        to_visit.pop();

        // si on atteint la destination on arrete
        if (current_node == end) {
            break;
        }

        // si le cout stocké est deja le meilleur on skip
        if (current_cost > distances[current_node].first) {
            continue;
        }

        for (auto const& edge : graph.get_neighbors(current_node)) {
            float const new_cost = current_cost + edge.weight;

            // Si on n'a pas encore visité ce noeud ou si y a un meilleur chemin
            auto const it = distances.find(edge.to);
            if (it == distances.end() || new_cost < it->second.first) {
                distances[edge.to] = {new_cost, current_node}; // coût + parent
                to_visit.push({new_cost, edge.to});
            }
        }
    }
    
    return distances;
}

std::vector<IDType> reconstruct_path(std::unordered_map<IDType, std::pair<float, IDType>> const& distances, IDType const start, IDType const end) {
    std::vector<IDType> path { };
    IDType current_node { end };
    while (current_node != start) {
        path.push_back(current_node);

        auto const find_current_node { distances.find(current_node) };
        if(find_current_node == distances.end()) {
            // if we reach a node that is not in the distances map, it means that there is no path from start to end
            return {};
        }
        else {
            // we update the current node to its parent node
            current_node = find_current_node->second.second;
        }
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<IDType> dijkstra_path(WeightedGraph const& graph, IDType const start, IDType const end) {
    return reconstruct_path(Dijkstra(graph, start, end), start, end);
}