#include "space.hpp"


std::unordered_set<size_t> disk(const Graph& graph, Point center, Real radius)
{
    std::unordered_set<size_t> output;

    for (size_t i = 0 ; i < graph.size() ; i++)
        if (dist(center, graph[i]) <= radius)
            output.insert(i);

    return output;
}

std::pair<Real, Real> bound_dist(const Graph& graph)
{
    Real min_dist = dist(graph[0], graph[1]);
    Real max_dist = dist(graph[0], graph[1]);

    for (size_t x = 0 ; x < graph.size() ; x++) {
        for (size_t y = 0 ; y < x ; y++) {
            const Real curr_dist = dist(graph[x], graph[y]);

            if (curr_dist > 0) {
                min_dist = std::min(min_dist, curr_dist);
                max_dist = std::max(max_dist, curr_dist);
            }
        }
    }

    return {min_dist, max_dist};
}
