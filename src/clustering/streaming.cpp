#include "streaming.hpp"


Graph a4_streaming_clustering(std::istream& stream, int k, int nb_outliers,
    double alpha, double beta, double eta)
{
    // Initial values
    Graph batch = read_serialized(stream, k + nb_outliers + 1);
    Graph last_offline;

    std::unordered_set<Point, pointhash> free_points;
    std::unordered_map<Point, std::vector<Point>, pointhash> clusters;

    Real r = bound_dist(batch).first / 2;


    {
        // Add missing points to the first batch
        const Graph missing =
            read_serialized(stream, k * nb_outliers - batch.size());
        batch.insert(batch.end(), missing.begin(), missing.end());
        free_points.insert(batch.begin(), batch.end());
    }

    // Streaming algorithm
    while (batch.size() > 0) {

        // Drop any free points that are within distance ηr of cluster centers
        const auto fp_cpy = free_points;

        for (Point free_p : fp_cpy) {
            for (const auto& [clust_p, _] : clusters) {
                if (dist(free_p, clust_p) < eta * r) {
                    free_points.erase(free_p);
                }
            }
        }


        // Regroup clusters of size z+1
        bool added_cluster = true;

        while (added_cluster) {
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
        }

        // Check if free points can be covered
        const bool feasible_shape =
               clusters.size() <= (size_t) k
            && free_points.size() <= (k - clusters.size() + 1) * nb_outliers;

        if (feasible_shape) {
            const Graph fp_cpy (free_points.begin(), free_points.end());
            last_offline = a3_clustering(fp_cpy, k - clusters.size(),
                nb_outliers, eta * r);
        }

        if (feasible_shape && (last_offline.size() > 0 || fp_cpy.size() == 0)) {
            batch = read_serialized(stream, k * nb_outliers);
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
            (size_t) cluster_index > clusters.size();

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
