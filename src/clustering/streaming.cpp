#include "streaming.hpp"


Graph streaming_clustering(StreamingGraph& graph, int k, int nb_outliers,
    double alpha, double beta, double eta)
{
    Graph last_offline, buffer;

    std::unordered_set<Point, pointhash> free_points;
    std::unordered_map<Point, std::vector<Point>, pointhash> clusters;

    // Number of nodes we fetch from the graph at each step
    const int batch_size = std::max(1, k * nb_outliers);

    // Compute an initial radius
    graph.reset();
    buffer = graph.read(k + nb_outliers);
    Real r = bound_dist(buffer).first / 2;

    // Compute size of the graph for debugging purpose
    const size_t graph_size = graph.size();

    // Debug all parameters
    std::cerr << "Running streaming algorithm:\n";
    std::cerr << " - graph_size = " << graph_size << '\n';
    std::cerr << " - k = " << k << '\n';
    std::cerr << " - nb_outliers = " << nb_outliers << '\n';
    std::cerr << " - alpha = " << alpha << '\n';
    std::cerr << " - beta = " << beta << '\n';
    std::cerr << " - eta = " << eta << '\n';
    std::cerr << " - batch_size = " << batch_size << '\n';

    // Fetch the first batch
    graph.reset();
    buffer = graph.read(batch_size);
    free_points.insert(buffer.begin(), buffer.end());

    while (true) {
        std::cout << std::fixed << std::setprecision(4) << std::setfill('0')
            << '\r' << "Running  (r = " << r << ")   "
            << ProgressBar(graph.cursor, graph_size) << "(" << graph.cursor
            << "/" << graph_size << ")" << std::flush;

        // Drop any free points that are within distance ηr of cluster centers
        for (auto it_fp = free_points.begin() ; it_fp != free_points.end() ; ) {
            bool delete_fp = false;

            for (const auto& [clust_p, _] : clusters) {
                if (dist(*it_fp, clust_p) < eta * r) {
                    delete_fp = true;
                    break;
                }
            }

            if (delete_fp)
                it_fp = free_points.erase(it_fp);
            else
                it_fp++;
        }

        // Regroup clusters of size z+1 and radius βr
        bool added_cluster;

        do {
            added_cluster = false;

            for (Point new_center : free_points) {
                std::vector<Point> support_points;

                for (Point neighbour : free_points)
                    if (dist(new_center, neighbour) < beta * r)
                        support_points.push_back(neighbour);

                if (support_points.size() >= (size_t) nb_outliers + 1) {
                    added_cluster = true;
                    clusters[new_center] = support_points;

                    for (Point p : support_points)
                        free_points.erase(p);

                    break;
                }
            }
        } while (added_cluster);

        // Check if free points can be covered
        const bool feasible_shape =
               clusters.size() <= (size_t) k
            && free_points.size() <= (k - clusters.size() + 1) * nb_outliers;

        if (feasible_shape) {
            const Graph fp_cpy (free_points.begin(), free_points.end());
            last_offline = a3_clustering(fp_cpy, k - clusters.size(),
                nb_outliers, eta * r / 3);
        }

        // If it can be covered, read a new batch
        if (feasible_shape
            && (last_offline.size() > 0 || free_points.size() == 0))
        {
            buffer = graph.read(batch_size);

            if (buffer.empty())
                break;

            free_points.insert(buffer.begin(), buffer.end());
            continue;
        }

        // Increase radius and filter old clusters
        #ifndef NDEBUG
        std::cerr << "Cleaning " << clusters.size() << " clusters"
            << std::flush;
        #endif

        r = alpha * r;

        for (auto it = clusters.begin() ; it != clusters.end() ; ) {
            const auto& [center, supports] = *it;
            bool delete_cluster = false;

            // Check if the cluster conflicts with an other cluster that we kept
            for (auto kept_it = clusters.begin() ; kept_it != it ; kept_it++) {
                const auto& [kept_center, kept_supports] = *kept_it;

                if (dist(kept_supports, supports) < 2 * alpha * r) {
                    delete_cluster = true;
                    break;
                }
            }

            if (delete_cluster)
                it = clusters.erase(it);
            else
                it++;
        }

        #ifndef NDEBUG
        std::cerr << ", kept " << clusters.size() << std::endl;
        #endif
    }

    std::cout << '\n';  // ends the progressbar

    // Compute output
    Graph output;

    for (const auto& [center, _] : clusters)
        output.push_back(center);

    output.insert(output.end(), last_offline.begin(), last_offline.end());
    return output;
}
