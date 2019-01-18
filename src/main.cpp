#include <fstream>
#include <iostream>
#include <limits>
#include <memory>

#include <json/json.h>

#include "clustering/offline.hpp"
#include "clustering/streaming.hpp"
#include "clustering/evaluate.hpp"

#include "data.hpp"
#include "debug.hpp"
#include "space.hpp"


constexpr int nb_clust = 20;
constexpr int outliers = 50;
constexpr double alpha = 1.1;
constexpr double beta = 8;
constexpr double eta = 16;


int main()
{
    // Open graph file
    std::ifstream file;
    file.open("dataset.txt");
    StreamingGraph graph (file);

    Graph clusters = streaming_a4_clustering(graph, nb_clust, outliers, alpha,
        beta, eta);

    // ----------
    // This benchmarking part isn't properly streaming

    graph.reset();
    Graph graph_cpy = graph.read(std::numeric_limits<int>::max());
    file.close();

    // Find the radius of current solution
    const Real clusters_radius = cost(graph_cpy, outliers, clusters);
    std::cout << "Radius of current solution is " << clusters_radius << '\n';

    // Find a lower bound to optimal radius
    std::unique_ptr<Graph> counter_example = std::make_unique<Graph>();
    const Real bound = bound_radius(graph_cpy, nb_clust, outliers, 0.001, 0, 2,
        counter_example);
    std::cout << "There can't be a solution with radius " << bound << '\n';
    std::cout << "The approximation ratio is at most "
        << clusters_radius / bound << '\n';

    // ----------
    // Output some logs

    {
        Json::Value params;
        params["log_type"] = "streaming";
        params["alpha"] = alpha;
        params["beta"] = beta;
        params["eta"] = eta;

        std::ofstream logs;
        logs.open("logs/streaming.json");
        logs << soluce_infos(graph_cpy, nb_clust, outliers, clusters, params);
    }

    {
        Json::Value params;
        params["log_type"] = "counter_example";
        params["critical_radius"] = "bound";

        std::ofstream logs;
        logs.open("logs/counter_example.json");
        logs << soluce_infos(graph_cpy, nb_clust, outliers, *counter_example,
            params);
    }
}
