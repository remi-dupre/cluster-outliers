#include "clustering.hpp"


Real cost(const Graph& graph, int outliers, const Graph& clustering)
{
    if (outliers >= (int) graph.size())
        return 0;

    std::vector<Real> node_cost
        (graph.size(), std::numeric_limits<Real>::infinity());

    for (Point center : clustering)
        for (size_t x = 0 ; x < graph.size() ; x++)
            node_cost[x] = std::min(node_cost[x], dist(graph[x], center));

    std::sort(node_cost.begin(), node_cost.end());
    return node_cost[graph.size() - outliers - 1];
}

Graph a3_clustering(const Graph& graph, int k, int nb_outliers, Real radius)
{
    // Compute initial disks
    std::vector<std::unordered_set<size_t>> G(graph.size()); // disks of radius r
    std::vector<std::unordered_set<size_t>> E(graph.size()); // disks of radius 3r

    for (size_t i = 0 ; i < graph.size() ; i++) {
        G[i] = disk(graph, graph[i], radius);
        E[i] = disk(graph, graph[i], 3*radius);
    }

    // Compute clusters
    Graph clusters;
    size_t nb_covered = 0;

    while ((int) clusters.size() < k) {
        // Compute cluster of max weight
        size_t mw_index = 0;

        for (size_t i = 1 ; i < graph.size() ; i++)
            if (G[i].size() > G[mw_index].size())
                mw_index = i;

        // Update disks
        clusters.push_back(graph[mw_index]);
        nb_covered += E[mw_index].size();

        const std::unordered_set<size_t> to_remove = E[mw_index];

        for (size_t i = 0 ; i < graph.size() ; i++) {
            for (size_t x : to_remove) {
                E[i].erase(x);
                G[i].erase(x);
            }
        }
    }

    // Output
    if (nb_covered + nb_outliers < graph.size())
        clusters.clear();

    return clusters;
}

Graph a3_clustering(const Graph& graph, int k, int nb_outliers)
{
    // Find an uper bound for the radius
    auto [x_min, y_min] = graph[0];
    auto [x_max, y_max] = graph[0];

    for (auto [x, y] : graph) {
        x_min = std::min(x, x_min);
        y_min = std::min(y, y_min);
        x_max = std::max(x, x_max);
        y_max = std::max(y, y_max);
    }

    const auto [min_dist, max_dist] = bound_dist(graph);

    std::cerr << "Min dist is " << min_dist << std::endl;
    std::cerr << "Max dist is " << max_dist << std::endl;

    Real min_r = 0;
    Real max_r = max_dist;

    // Find a 3-approximation algorithm using a binary search
    while (max_r - min_r >= min_dist) {
        const Real mid_r = (min_r + max_r) / 2;
        const Graph& clustering = a3_clustering(graph, k, nb_outliers, mid_r);

        if (clustering.empty())
            min_r = mid_r;
        else
            max_r = mid_r;
    }

    std::cerr << "Radius is " << max_r << std::endl;

    return a3_clustering(graph, k, nb_outliers, max_r);
}
