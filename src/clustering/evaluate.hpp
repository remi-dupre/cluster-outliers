/**
 * Implement tools to evaluate an upper bound to the approximation ratio of a
 * solution.
 */
#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

#include <json/json.h>

#include "../data.hpp"
#include "../progressbar.hpp"
#include "../space.hpp"


/**
 * Check if the radius is feasible using a probabilistic algorithm. If this
 * function returns false, it means that the is no covering of the graph with
 * the given radius, if the function returns true, it means nothing.
 *
 * If counter_example is not nullptr, the graph it points to will be set to a
 * counter example for the returned value.
 */
bool feasible_radius(Graph graph, int k, int nb_outliers, Real radius,
    const std::unique_ptr<Graph>& counter_example =  nullptr);

/**
 * Estimate the minimal radius needed to cover the graph.
 *
 * If counter_example is not nullptr, the graph it points to will be set to a
 * counter example for the returned value.
 */
Real bound_radius(const Graph& graph, int k, int nb_outliers,
    Real precision = 0.01, Real lower_bound = 0, Real upper_bound = 5,
    const std::unique_ptr<Graph>& counter_example =  nullptr);
