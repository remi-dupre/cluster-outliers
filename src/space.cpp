#include "space.hpp"


Real dist(Point a, const std::vector<Point>& s)
{
    Real min_dist = std::numeric_limits<Real>::infinity();

    for (Point b : s)
        min_dist = std::min(min_dist, dist(a, b));

    return min_dist;
}

std::unordered_set<size_t> disk(const Graph& graph, Point center, Real radius)
{
    std::unordered_set<size_t> output;

    for (size_t i = 0 ; i < graph.size() ; i++)
        if (dist(center, graph[i]) <= radius)
            output.insert(i);

    return output;
}
