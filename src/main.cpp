#include <iostream>

#include "clustering/offline.hpp"
#include "clustering/streaming.hpp"

#include "data.hpp"
#include "space.hpp"


int main()
{
    // Graph graph = read_serialized(std::cin);
    // Graph cluster = a3_clustering(graph, 10, 10);
    //
    // std::cout << "Solution cost: " << cost(graph, 10, cluster) << '\n';

    Graph cluster = streaming_a4_clustering(std::cin, 20, 100);
    std::cout << "Solution size: " << cluster.size() << '\n';

    // streaming_check_soluce(std::cin);

    return 0;
}
