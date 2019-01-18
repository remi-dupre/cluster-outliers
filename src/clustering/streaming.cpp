#include "streaming.hpp"


Graph streaming_a4_clustering(StreamingGraph& graph, int k, int nb_outliers,
    double alpha, double beta, double eta)
{

    // Initial values
    Graph last_offline, buffer;

    std::unordered_set<Point, pointhash> free_points;
    std::unordered_map<Point, std::vector<Point>, pointhash> clusters;

    const int batch_size = std::min(100, std::max(1, k * nb_outliers));

    graph.reset();
    buffer = graph.read(k + nb_outliers);
    Real r = bound_dist(buffer.begin(), buffer.end()).first / 2;

    const size_t graph_size = graph.size();

    // Debug some informations
    std::cerr << "Running streaming algorithm:\n";
    std::cerr << " - graph_size = " << graph_size << '\n';
    std::cerr << " - k = " << k << '\n';
    std::cerr << " - nb_outliers = " << nb_outliers << '\n';
    std::cerr << " - alpha = " << alpha << '\n';
    std::cerr << " - beta = " << beta << '\n';
    std::cerr << " - eta = " << eta << '\n';
    std::cerr << " - batch_size = " << batch_size << '\n';

    graph.reset();

    // Add missing points to the first batch
    buffer = graph.read(batch_size);
    free_points.insert(buffer.begin(), buffer.end());

    // Streaming algorithm
    while (true) {
        std::cout << std::fixed << std::setprecision(4) << std::setfill('0')
            << '\r' << "Running  (r = " << r << ")   "
            << ProgressBar(graph.cursor, graph_size) << "(" << graph.cursor
            << "/" << graph_size << ")" << std::flush;

        // Drop any free points that are within distance ηr of cluster centers
        {
            const auto fp_cpy = free_points;

            for (Point free_p : fp_cpy)
                for (const auto& [clust_p, _] : clusters)
                    if (dist(free_p, clust_p) < eta * r)
                        free_points.erase(free_p);
        }

        // Regroup clusters of size z+1
        bool added_cluster;

        do {
            added_cluster = false;

            for (Point new_center : free_points) {
                std::vector<Point> support_points;

                for (Point neighbour : free_points) {
                    if (dist(new_center, neighbour) < beta*r) {
                        support_points.push_back(neighbour);
                    }
                }

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
                nb_outliers, eta * r);
        }

        // If it can be covered, read a new batch
        if (feasible_shape && (last_offline.size() > 0 || free_points.size() == 0)) {
            buffer = graph.read(batch_size);

            if (buffer.empty())
                break;

            free_points.insert(buffer.begin(), buffer.end());
            continue;
        }

        // Increase radius and filter old clusters
        r = alpha * r;
        std::unordered_set<Point, pointhash> marked_centers;

        #ifndef NDEBUG
        std::cerr << "Cleaning " << clusters.size() << " clusters"
            << std::flush;
        #endif

        while (marked_centers.size() < clusters.size()) {
            for (const auto& [center, supports] : clusters) {
                if (marked_centers.count(center) == 0) {
                    marked_centers.insert(center);
                    const auto clusters_cpy = clusters;

                    for (const auto& [other_center, other_supports] : clusters_cpy) {
                        if (other_center != center)
                            for (const Point p1 : supports)
                                for (const Point p2 : other_supports)
                                    if (dist(p1, p2) < 2*alpha*r)
                                        clusters.erase(other_center);

                        if (clusters_cpy.size() > clusters.size())
                            break;
                    }
                }
            }
        }

        #ifndef NDEBUG
        std::cerr << ", kept " << clusters.size() << std::endl;
        #endif
    }

    std::cout << '\n';

    // Compute output
    Graph output;

    for (const auto& [center, _] : clusters)
        output.push_back(center);

    output.insert(output.end(), last_offline.begin(), last_offline.end());
    return output;
}
