#include "evaluate.hpp"


bool feasible_radius(Graph graph, int k, int nb_outliers, Real radius,
    std::default_random_engine& engine,
    const std::unique_ptr<Graph>& counter_example)
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

    if (arbitrary_cover.size() > (size_t) k + nb_outliers)
        *counter_example = arbitrary_cover;

    return arbitrary_cover.size() <= (size_t) k + nb_outliers;
}

Real bound_radius(const Graph& graph, int k, int nb_outliers, Real precision,
    Real lower_bound, Real upper_bound,
    const std::unique_ptr<Graph>& counter_example)
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

        if (!feasible_radius(graph, k, nb_outliers, radius, random_engine,
          counter_example))
            min_rad = radius;
        else
            max_rad = radius;
    }

    std::cout << '\n';
    return min_rad;
}
