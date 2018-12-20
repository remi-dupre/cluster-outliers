#include "space.hpp"


std::set<size_t> disk(const Graph& graph, Point center, Real radius)
{
    std::set<size_t> output;

    for (size_t i = 0 ; i < graph.size() ; i++)
        if (dist(center, graph[i]) <= radius)
            output.insert(i);

    return output;
}
