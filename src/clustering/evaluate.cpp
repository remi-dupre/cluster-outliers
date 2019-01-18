#include "evaluate.hpp"


bool feasible_radius(Graph graph, int k, int nb_outliers, Real radius,
    const std::unique_ptr<Graph>& counter_example)
{
    Graph arbitrary_cover;
    std::sort(graph.begin(), graph.end());

    #pragma omp parallel
    {
        Graph local_arbitrary_cover;

        #pragma omp for
        for (int i_select = 0 ; i_select < graph.size() ; i_select++) {
            if (dist(graph[i_select], local_arbitrary_cover) > 2 * radius) {
                #pragma omp critical
                {
                    if (dist(graph[i_select], arbitrary_cover) > 2 * radius)
                        arbitrary_cover.push_back(graph[i_select]);

                    local_arbitrary_cover = arbitrary_cover;
                }
            }
        }
    }

    if (arbitrary_cover.size() > (size_t) k + nb_outliers)
        *counter_example = arbitrary_cover;

    return arbitrary_cover.size() <= (size_t) k + nb_outliers;
}

Real bound_radius(const Graph& graph, int k, int nb_outliers, Real precision,
    Real lower_bound, Real upper_bound,
    const std::unique_ptr<Graph>& counter_example)
{
    Real min_rad = lower_bound;
    Real max_rad = upper_bound;

    while (max_rad - min_rad > precision) {
        const Real radius = (max_rad + min_rad) / 2;

        int max_step = log((upper_bound - lower_bound) / precision) / log(2);
        int step = 1 + max_step - log((max_rad - min_rad) / precision) / log(2);
        std::cout << '\r' << "Evaluates r = " << radius
            << ProgressBar(step, max_step) << '(' << step << '/' << max_step
            << ')' << std::flush;

        if (!feasible_radius(graph, k, nb_outliers, radius, counter_example))
            min_rad = radius;
        else
            max_rad = radius;
    }

    std::cout << '\n';
    return min_rad;
}
