#include <iostream>

#include "clustering.hpp"
#include "data.hpp"
#include "space.hpp"


int main()
{
    Graph graph = read_serialized(std::cin);
    Graph cluster = a3_clustering(graph, 10, 10);

    std::cout << cluster.size() << '\n';

    return 0;
}
