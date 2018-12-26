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

    Graph cluster = a4_streaming_clustering(std::cin, 50, 10);
    std::cout << "Solution size: " << cluster.size() << '\n';

    return 0;
}
