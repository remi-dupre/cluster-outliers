#include "debug.hpp"


Json::Value soluce_infos(const Graph& graph, int k, int nb_outliers,
    const Graph& clusters, Json::Value params)
{
    params["outliers"] = nb_outliers;
    params["clusters"] = k;

    Json::Value log_custers;
    int cluster_index = 0;

    for (Point center : clusters) {
        log_custers[cluster_index]["center"][0] = center.first;
        log_custers[cluster_index]["center"][1] = center.second;
        cluster_index++;
    }

    Json::Value logs;
    logs["parameters"] = params;
    logs["radius"] = cost(graph, nb_outliers, clusters);
    logs["clusters"] = log_custers;

    return logs;
}
