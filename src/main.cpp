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

    // Graph cluster = streaming_a4_clustering(std::cin, 20, 100);
    // std::cout << "Solution size: " << cluster.size() << '\n';

    Graph graph = read_serialized(std::cin);
    const Real bound = bound_radius(graph, 20, 100, 0.001);
    std::cout << "There can't be a solution with radius " << bound << '\n';

    // streaming_check_soluce(std::cin);

    return 0;
}
