#include <fstream>
#include <iostream>

#include "clustering/offline.hpp"
#include "clustering/streaming.hpp"
#include "clustering/evaluate.hpp"

#include "data.hpp"
#include "space.hpp"


int main()
{
    // Graph graph = read_serialized(std::cin);
    // Graph cluster = a3_clustering(graph, 10, 10);
    //
    // std::cout << "Solution cost: " << cost(graph, 10, cluster) << '\n';


    // Graph graph = read_serialized(std::cin);
    //
    // Graph cluster = streaming_a4_clustering(graph, 20, 100);
    // std::cout << "Solution size: " << cluster.size() << '\n';
    //
    // check_soluce(graph);
    //
    // const Real bound = bound_radius(graph, 20, 100, 0.001);
    // std::cout << "There can't be a solution with radius " << bound << '\n';


    // ------------


    std::ifstream file;
    file.open("dataset.txt");
    StreamingGraph graph (file);

    Graph cluster = streaming_a4_clustering(graph, 20, 100);

    // auto p = g2.read();
    // std::cout << p[0].first << ' ' << p[0].second << '\n';
    // p = g2.read();
    // std::cout << p[0].first << ' ' << p[0].second << '\n';


    // StreamingGraphIterator it(g2);
    // std::cout << it->first << ' ' << it->second << '\n';
    // it++;
    // std::cout << it->first << ' ' << it->second << '\n';

    // for (Point p : g2) {
    //     std::cout << p.first << ' ' << p.second << '\n';
    // }

    return 0;
}
