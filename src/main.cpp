#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include <json/json.h>

#include "clustering/offline.hpp"
#include "clustering/streaming.hpp"
#include "clustering/evaluate.hpp"

#include "data.hpp"
#include "debug.hpp"
#include "space.hpp"


constexpr int default_nb_clust = 20;
constexpr int default_outliers = 10;
constexpr double alpha = 1.1;
constexpr double beta = 8;
constexpr double eta = 16;


int main(int argc, const char** argv)
{
    // ----------
    // Read parameters

    if (argc < 2) {
        std::cout << "Please, specify a dataset file\n";
        return 0;
    }

    const std::string dataset = argv[1];
    const int nb_clust = (argc > 2) ? atoi(argv[2]) : default_nb_clust;
    const int outliers = (argc > 3) ? atoi(argv[3]) : default_outliers;

    if (nb_clust < 0 || outliers < 0) {
        std::cerr << "Invalid parameters\n";

    }

    // ----------
    // Open graph file

    std::ifstream file;
    file.open(dataset);

    if (!file.is_open()) {
        std::cerr << "Couldn't open specified file: " << dataset << "\n";
        return 1;
    }

    StreamingGraph graph (file);

    // ----------
    // Run the streaming algorithm

    const auto start = std::chrono::system_clock::now();
    Graph clusters = streaming_a4_clustering(graph, nb_clust, outliers, alpha,
        beta, eta);
    const auto end = std::chrono::system_clock::now();

    const std::chrono::duration<double> duration = end - start;

    // ----------
    // This benchmarking part isn't properly streaming

    graph.reset();
    Graph graph_cpy = graph.read(std::numeric_limits<int>::max());
    file.close();

    // Find the radius of current solution
    const Real clusters_radius = cost(graph_cpy, outliers, clusters);

    // Find a lower bound to optimal radius
    std::unique_ptr<Graph> counter_example = std::make_unique<Graph>();
    const Real bound = bound_radius(graph_cpy, nb_clust, outliers, 0.001, 0, 2,
        counter_example);

    std::cout << "Radius of current solution is " << clusters_radius << '\n';
    std::cout << "There can't be a solution with radius " << bound << '\n';
    std::cout << "The approximation ratio is at most "
        << clusters_radius / bound << '\n';

    // ----------
    // Output some logs

    {
        Json::Value params;
        params["log_type"] = "streaming";

        params["running_time"] = duration.count();
        params["approx_ratio"] = clusters_radius / bound;

        params["alpha"] = alpha;
        params["beta"] = beta;
        params["eta"] = eta;

        const auto filename = "logs/streaming_" + std::to_string(nb_clust)
        + "-" + std::to_string(outliers) + ".json";

        std::ofstream logs;
        logs.open(filename);
        logs << soluce_infos(graph_cpy, nb_clust, outliers, clusters, params);
    }

    {
        Json::Value params;
        params["log_type"] = "counter_example";
        params["critical_radius"] = bound;

        const auto filename = "logs/counter_expl_" + std::to_string(nb_clust)
        + "-" + std::to_string(outliers) + ".json";

        std::ofstream logs;
        logs.open(filename);
        logs << soluce_infos(graph_cpy, nb_clust, outliers, *counter_example,
            params);
    }
}
