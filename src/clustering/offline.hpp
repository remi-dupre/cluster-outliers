#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

#include "../space.hpp"


/**
 * Implementation of a simple k-clustering with outliers algorithm as defined
 * in section 3 of "Algorithms for Facility Location Problems with Outliers"
 * from Moses Charikar, Samir Khuller, David M. Mount, Giri Narasimhan
 */

/**
 * Try to find a k-clustering with at most nb_outliers outliers using only
 * clusters of radius at most 3*radius.
 * If there is no such clustering, return an empty graph.
 *
 * Time: O(kn ln n) with n = graph.size()
 */
Graph a3_clustering(const Graph& graph, int k, int nb_outliers, Real radius);

/**
 * Find a 3-approximation k-clustering with at most nb_outliers outliers.
 *
 * Time: O(kn ln n ln(max_dist/min_dist) + n²)
 */
Graph a3_clustering(const Graph& graph, int k, int nb_outliers);
