/**
 * Implement tools to evaluate an upper bound to the approximation ratio of a
 * solution.
 */
#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include <json/json.h>

#include "../data.hpp"
#include "../progressbar.hpp"
#include "../space.hpp"


/**
 * Check if the solution saved in 'logs/streaming.json' is correct using
 * parameters and radius saved in this file.
 */
bool check_soluce(const Graph& graph);

/**
 * Check if the radius is feasible using a probabilistic algorithm. If this
 * function returns false, it means that the is no covering of the graph with
 * the given radius, if the function returns true, it means nothing.
 */
bool feasible_radius(Graph graph, int k, int nb_outliers, Real radius,
    std::default_random_engine &engine);

/**
 * Estimate the minimal radius needed to cover the graph.
 */
Real bound_radius(const Graph& graph, int k, int nb_outliers,
    Real precision = 0.01, Real lower_bound = 0, Real upper_bound = 5);
