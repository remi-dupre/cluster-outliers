#include "evaluate.hpp"


bool check_soluce(const Graph& graph)
{
    Json::Value logs;
    std::ifstream logs_file("logs/streaming.json");
    Json::parseFromStream(Json::CharReaderBuilder(), logs_file, &logs, nullptr);

    Real r = logs["radius"].asDouble();
    Real eta = logs["parameters"]["eta"].asDouble();

    size_t out_count = 0;

    for (Point p : graph) {
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

bool feasible_radius(Graph graph, int k, int nb_outliers, Real radius,
    std::default_random_engine& engine)
{
    std::vector<Point> arbitrary_cover;

    for (int step = 0 ; step < k + nb_outliers + 1 ; step++) {
        std::shuffle(graph.begin(), graph.end(), engine);
        size_t i_select = graph.size();

        #pragma omp parallel for
        for (size_t i = 0 ; i < graph.size() ; i++) {
            if (i_select < graph.size())
                continue;

            if (dist(graph[i], arbitrary_cover) > 2 * radius) {
                #pragma omp critical
                i_select = i;
            }
        }

        if (i_select < graph.size())
            arbitrary_cover.push_back(graph[i_select]);
        else
            break;
    }

    return arbitrary_cover.size() <= (size_t) k + nb_outliers;
}

Real bound_radius(const Graph& graph, int k, int nb_outliers, Real precision,
    Real lower_bound, Real upper_bound)
{
    std::random_device rd;
    std::default_random_engine random_engine(rd());

    Real min_rad = lower_bound;
    Real max_rad = upper_bound;

    while (max_rad - min_rad > precision) {
        const Real radius = (max_rad + min_rad) / 2;

        int max_step = log((upper_bound - lower_bound) / precision) / log(2);
        int step = 1 + max_step - log((max_rad - min_rad) / precision) / log(2);
        std::cout << '\r' << "Evaluates r = " << radius
            << ProgressBar(step, max_step) << '(' << step << '/' << max_step
            << ')' << std::flush;

        if (!feasible_radius(graph, k, nb_outliers, radius, random_engine))
            min_rad = radius;
        else
            max_rad = radius;
    }

    std::cout << '\n';
    return min_rad;
}
