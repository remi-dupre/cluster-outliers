#include "space.hpp"


Real dist(Point a, const std::vector<Point>& s)
{
    Real min_dist = std::numeric_limits<Real>::infinity();

    for (Point b : s)
        min_dist = std::min(min_dist, dist(a, b));

    return min_dist;
}

Real dist(const std::vector<Point>& s, const std::vector<Point>& p)
{
    Real min_dist = std::numeric_limits<Real>::infinity();

    for (Point a : s)
        min_dist = std::min(min_dist, dist(a, p));

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

std::pair<Real, Real> bound_dist(const Graph& graph)
{
    Real min_dist = +std::numeric_limits<Real>::infinity();
    Real max_dist = -std::numeric_limits<Real>::infinity();

    #pragma omp parallel for
    for (size_t i_x = 0 ; i_x < graph.size() ; i_x++) {
        for (size_t i_y = 0 ; i_y < i_x ; i_y++) {
            const Real curr_dist = dist(graph[i_x], graph[i_y]);
            min_dist = std::min(min_dist, curr_dist);
            max_dist = std::max(max_dist, curr_dist);
        }
    }

    return {min_dist, max_dist};
}
