#include <iostream>

#include "clustering.hpp"
#include "data.hpp"
#include "space.hpp"


int main()
{
    Graph graph = read_serialized(std::cin);
    Graph cluster = a3_clustering(graph, 10, 10);

    std::cout << "Solution cost: " << cost(graph, 10, cluster) << '\n';

    return 0;
}
