#include "streaming.hpp"


Graph streaming_a4_clustering(std::istream& stream, int k, int nb_outliers,
    double alpha, double beta, double eta)
{
    // Initial values
    Graph batch = read_serialized(stream, k + nb_outliers + 1);
    Graph last_offline;

    std::unordered_set<Point, pointhash> free_points;
    std::unordered_map<Point, std::vector<Point>, pointhash> clusters;

    const int batch_size = std::min(100, std::max(1, k * nb_outliers));
    Real r = bound_dist(batch).first / 2;

    // Debug some informations
    std::cerr << "Running streaming algorithm:\n";
    std::cerr << " - k = " << k << '\n';
    std::cerr << " - nb_outliers = " << nb_outliers << '\n';
    std::cerr << " - alpha = " << alpha << '\n';
    std::cerr << " - beta = " << beta << '\n';
    std::cerr << " - eta = " << eta << '\n';
    std::cerr << " - batch_size = " << batch_size << '\n';

    {
        // Add missing points to the first batch
        const Graph missing =
            read_serialized(stream, batch_size - batch.size());
        batch.insert(batch.end(), missing.begin(), missing.end());
        free_points.insert(batch.begin(), batch.end());
     }

    // Streaming algorithm
    while (batch.size() > 0) {

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

        // If it can't be covered, read a new batch
        if (feasible_shape && (last_offline.size() > 0 || free_points.size() == 0)) {
            batch = read_serialized(stream, batch_size);
            free_points.insert(batch.begin(), batch.end());
            continue;
        }

        // Increase radius and filter old clusters
        std::cerr << "Increase radius to " << r << std::endl;
        r = alpha * r;

        std::cerr << "Cleaning " << clusters.size() << " clusters" << std::flush;
        std::unordered_set<Point, pointhash> marked_centers;

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

        std::cerr << ", kept " << clusters.size() << std::endl;
    }

    // Compute output
    Graph output;

    for (const auto& [center, _] : clusters)
        output.push_back(center);

    output.insert(output.end(), last_offline.begin(), last_offline.end());

    // Print logs if corresponding dir exists
    Json::Value params;
    params["alpha"] = alpha;
    params["beta"] = beta;
    params["eta"] = eta;
    params["outliers"] = nb_outliers;
    params["clusters"] = k;

    Json::Value log_custers;
    int cluster_index = 0;

    for (Point center : output) {
        log_custers[cluster_index]["center"][0] = center.first;
        log_custers[cluster_index]["center"][1] = center.second;
        log_custers[cluster_index]["free_point"] =
            (size_t) cluster_index >= clusters.size();

        cluster_index++;
    }

    Json::Value logs;
    logs["radius"] = r;
    logs["parameters"] = params;
    logs["clusters"] = log_custers;

    std::ofstream log_stream("./logs/streaming.json", std::ios::trunc);
    log_stream << logs;

    return output;
}

bool streaming_check_soluce(std::istream& stream)
{
    Json::Value logs;
    std::ifstream logs_file("logs/streaming.json");
    Json::parseFromStream(Json::CharReaderBuilder(), logs_file, &logs, nullptr);

    Real r = logs["radius"].asDouble();
    Real eta = logs["parameters"]["eta"].asDouble();

    size_t out_count = 0;

    for (Point p : read_serialized(stream)) {
        bool inside = false;

        for (const Json::Value& j_cluster : logs["clusters"]) {
            Point cluster = {
                j_cluster["center"][0].asDouble(),
                j_cluster["center"][1].asDouble()
            };

            if (dist(p, cluster) <= r * eta)
                inside = true;
        }

        if (!inside)
            out_count++;
    }

    std::cerr << "Found " << out_count << " outliers\n";

    return out_count <= logs["outliers"].asUInt();
}
